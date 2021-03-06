/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ixion/formula_lexer.hpp"
#include "ixion/global.hpp"

#include <cassert>
#include <iostream>
#include <sstream>

#include <boost/noncopyable.hpp>

#define IXION_DEBUG_LEXER 0

using namespace std;

namespace ixion {

class tokenizer : public ::boost::noncopyable
{
    enum buffer_type {
        buf_numeral,
        buf_name
    };

public:
    explicit tokenizer(lexer_tokens_t& tokens, const char* p, size_t n) :
        m_tokens(tokens),
        m_sep_arg(','),
        m_sep_decimal('.'),
        mp_first(p),
        mp_char(NULL),
        m_size(n),
        m_pos(0),
        mp_char_stored(NULL),
        m_pos_stored(0),
        m_scope(0)
    {
    }

    void run();

private:
    bool is_arg_sep(char c) const;
    bool is_decimal_sep(char c) const;
    bool is_op(char c) const;

    void init();

    void numeral();
    void space();
    void name();
    void op(lexer_opcode_t oc);
    void string();

    bool has_char() const;
    void next();
    void push_pos();
    void pop_pos();

private:
    lexer_tokens_t& m_tokens;

    char m_sep_arg;
    char m_sep_decimal;

    const char* mp_first;
    const char* mp_char;
    const size_t m_size;
    size_t m_pos;

    const char* mp_char_stored;
    size_t m_pos_stored;
    size_t m_scope;
};

void tokenizer::init()
{
    m_tokens.clear();
    mp_char = mp_first;
    m_pos = 0;
}

void tokenizer::run()
{
    if (!m_size)
        // Nothing to do.
        return;

    init();

    while (has_char())
    {
        if (is_digit(*mp_char))
        {
            numeral();
            continue;
        }

        if (!is_op(*mp_char))
        {
            name();
            continue;
        }

        if (is_arg_sep(*mp_char))
        {
            op(op_sep);
            continue;
        }

        switch (*mp_char)
        {
            case ' ':
                space();
                break;
            case '+':
                op(op_plus);
                break;
            case '-':
                op(op_minus);
                break;
            case '/':
                op(op_divide);
                break;
            case '*':
                op(op_multiply);
                break;
            case '=':
                op(op_equal);
                break;
            case '<':
                op(op_less);
                break;
            case '>':
                op(op_greater);
                break;
            case '(':
                op(op_open);
                break;
            case ')':
                op(op_close);
                break;
            case '"':
                string();
                break;
        }
    }
}

bool tokenizer::is_arg_sep(char c) const
{
    return c == m_sep_arg;
}

bool tokenizer::is_decimal_sep(char c) const
{
    return c == m_sep_decimal;
}

bool tokenizer::is_op(char c) const
{
    if (m_scope == 0 && is_arg_sep(c))
        return true;

    switch (*mp_char)
    {
        case ' ':
        case '+':
        case '-':
        case '/':
        case '*':
        case '(':
        case ')':
        case '"':
        case '=':
        case '<':
        case '>':
            return true;
    }
    return false;
}

void tokenizer::numeral()
{
    const char* p = mp_char;
    push_pos();

    size_t len = 1;
    size_t sep_count = 0;
    for (next(); has_char(); next(), ++len)
    {
        if (*mp_char == ':')
        {
            // Treat this as a name.  This may be a part of a row-only range (e.g. 3:3).
            pop_pos();
            name();
            return;
        }

        if (is_digit(*mp_char))
            continue;
        if (is_decimal_sep(*mp_char) && ++sep_count <= 1)
            continue;

        break;
    }

    if (sep_count > 1)
    {
        ostringstream os;
        os << "error parsing numeral: " << std::string(p, len);
        throw formula_lexer::tokenize_error(os.str());
    }
    double val = global::to_double(p, len);
    m_tokens.push_back(make_unique<lexer_value_token>(val));
}

void tokenizer::space()
{
    // space is ignored for now.
    next();
}

void tokenizer::name()
{
    assert(m_scope == 0);

    const char* p = mp_char;
    char c = *mp_char;
    if (c == '[')
        ++m_scope;
    else if (c == ']')
    {
        m_tokens.push_back(make_unique<lexer_name_token>(p, 1));
        next();
        return;
    }

    size_t len = 1;
    for (next(); has_char(); next(), ++len)
    {
        c = *mp_char;
        if (c == '[')
        {
            ++m_scope;
        }
        else if (c == ']')
        {
            if (!m_scope)
                break;

            --m_scope;
        }
        else if (is_op(c))
            break;
    }

    m_tokens.push_back(make_unique<lexer_name_token>(p, len));
}

void tokenizer::op(lexer_opcode_t oc)
{
    m_tokens.push_back(make_unique<lexer_token>(oc));
    next();
}

void tokenizer::string()
{
    next();
    const char* p = mp_char;
    size_t len = 0;
    for (; *mp_char != '"' && has_char(); ++len)
        next();

    if (len)
        m_tokens.push_back(make_unique<lexer_string_token>(p, len));

    if (*mp_char == '"')
        next();
}

void tokenizer::next()
{
    ++mp_char;
    ++m_pos;
}

void tokenizer::push_pos()
{
    mp_char_stored = mp_char;
    m_pos_stored = m_pos;
}

void tokenizer::pop_pos()
{
    mp_char = mp_char_stored;
    m_pos = m_pos_stored;

    mp_char_stored = NULL;
    m_pos_stored = 0;
}

bool tokenizer::has_char() const
{
    return m_pos < m_size;
}

// ============================================================================

formula_lexer::tokenize_error::tokenize_error(const string& msg) : general_error(msg) {}

formula_lexer::formula_lexer(const char* p, size_t n) :
    mp_first(p), m_size(n) {}

formula_lexer::~formula_lexer() {}

void formula_lexer::tokenize()
{
#if IXION_DEBUG_LEXER
    __IXION_DEBUG_OUT__ << "formula string: '" << std::string(mp_first, m_size) << "'" << endl;
#endif
    tokenizer tkr(m_tokens, mp_first, m_size);
    tkr.run();
#if IXION_DEBUG_LEXER
    __IXION_DEBUG_OUT__ << print_tokens(m_tokens, true) << endl;
#endif
}

void formula_lexer::swap_tokens(lexer_tokens_t& tokens)
{
    m_tokens.swap(tokens);
}

}
/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
