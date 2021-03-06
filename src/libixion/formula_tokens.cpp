/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ixion/formula_tokens.hpp"
#include "ixion/exceptions.hpp"

using ::std::string;

namespace ixion {

const char* get_opcode_name(fopcode_t oc)
{
    switch (oc)
    {
        case fop_close:
            return "close";
        case fop_divide:
            return "divide";
        case fop_err_no_ref:
            return "error no ref";
        case fop_minus:
            return "minus";
        case fop_multiply:
            return "multiply";
        case fop_open:
            return "open";
        case fop_plus:
            return "plus";
        case fop_sep:
            return "separator";
        case fop_single_ref:
            return "single ref";
        case fop_range_ref:
            return "range ref";
        case fop_named_expression:
            return "named expression";
        case fop_string:
            return "string";
        case fop_value:
            return "value";
        case fop_function:
            return "function";
        default:
            ;
    }
    return "unknown";
}

const char* get_formula_opcode_string(fopcode_t oc)
{
    switch (oc)
    {
        case fop_close:
            return ")";
        case fop_divide:
            return "/";
        case fop_minus:
            return "-";
        case fop_multiply:
            return "*";
        case fop_open:
            return "(";
        case fop_plus:
            return "+";
        case fop_sep:
            return ",";
        case fop_equal:
            return "=";
        case fop_not_equal:
            return "<>";
        case fop_less:
            return "<";
        case fop_less_equal:
            return "<=";
        case fop_greater:
            return ">";
        case fop_greater_equal:
            return ">=";
        case fop_string:
        case fop_value:
        case fop_function:
        case fop_err_no_ref:
        case fop_single_ref:
        case fop_range_ref:
        case fop_named_expression:
        default:
            ;
    }
    return "";
}

// ============================================================================

formula_token_base::formula_token_base(fopcode_t op) :
    m_opcode(op)
{
}

formula_token_base::formula_token_base(const formula_token_base& r) :
    m_opcode(r.m_opcode)
{
}

formula_token_base::~formula_token_base()
{
}

fopcode_t formula_token_base::get_opcode() const
{
    return m_opcode;
}

bool formula_token_base::operator== (const formula_token_base& r) const
{
    if (m_opcode != r.m_opcode)
        return false;

    switch (m_opcode)
    {
        case fop_close:
        case fop_divide:
        case fop_minus:
        case fop_multiply:
        case fop_open:
        case fop_plus:
        case fop_sep:
            return true;
        case fop_single_ref:
            return get_single_ref() == r.get_single_ref();
        case fop_range_ref:
            return get_range_ref() == r.get_range_ref();
        case fop_named_expression:
            return get_name() == r.get_name();
        case fop_string:
            return get_index() == r.get_index();
        case fop_value:
            return get_value() == r.get_value();
        case fop_function:
            return get_index() == r.get_index();
        default:
            ;
    }
    return false;
}

bool formula_token_base::operator!= (const formula_token_base& r) const
{
    return !operator== (r);
}

address_t formula_token_base::get_single_ref() const
{
    return address_t();
}

range_t formula_token_base::get_range_ref() const
{
    return range_t();
}

table_t formula_token_base::get_table_ref() const
{
    return table_t();
}

double formula_token_base::get_value() const
{
    return 0.0;
}

size_t formula_token_base::get_index() const
{
    return 0;
}

std::string formula_token_base::get_name() const
{
    return std::string();
}

// ============================================================================

opcode_token::opcode_token(fopcode_t oc) :
    formula_token_base(oc)
{
}

opcode_token::opcode_token(const opcode_token& r) :
    formula_token_base(r)
{
}

opcode_token::~opcode_token()
{
}

// ============================================================================

value_token::value_token(double value) :
    formula_token_base(fop_value),
    m_value(value)
{
}

value_token::~value_token()
{
}

double value_token::get_value() const
{
    return m_value;
}

string_token::string_token(size_t str_identifier) :
    formula_token_base(fop_string),
    m_str_identifier(str_identifier) {}

string_token::~string_token() {}

size_t string_token::get_index() const
{
    return m_str_identifier;
}

// ============================================================================

single_ref_token::single_ref_token(const address_t& addr) :
    formula_token_base(fop_single_ref),
    m_address(addr)
{
}

single_ref_token::single_ref_token(const single_ref_token& r) :
    formula_token_base(r),
    m_address(r.m_address)
{
}

single_ref_token::~single_ref_token()
{
}

address_t single_ref_token::get_single_ref() const
{
    return m_address;
}

// ============================================================================

range_ref_token::range_ref_token(const range_t& range) :
    formula_token_base(fop_range_ref),
    m_range(range)
{
}

range_ref_token::range_ref_token(const range_ref_token& r) :
    formula_token_base(r),
    m_range(r.m_range)
{
}

range_ref_token::~range_ref_token()
{
}

range_t range_ref_token::get_range_ref() const
{
    return m_range;
}

table_ref_token::table_ref_token(const table_t& table) :
    formula_token_base(fop_table_ref),
    m_table(table) {}

table_ref_token::table_ref_token(const table_ref_token& r) :
    formula_token_base(r),
    m_table(r.m_table) {}

table_ref_token::~table_ref_token() {}

table_t table_ref_token::get_table_ref() const
{
    return m_table;
}

named_exp_token::named_exp_token(const char* p, size_t n) :
    formula_token_base(fop_named_expression),
    m_name(p, n) {}

named_exp_token::named_exp_token(const named_exp_token& r) :
    formula_token_base(r),
    m_name(r.m_name) {}

named_exp_token::~named_exp_token() {}

string named_exp_token::get_name() const
{
    return m_name;
}

// ============================================================================

function_token::function_token(size_t func_oc) :
    formula_token_base(fop_function),
    m_func_oc(func_oc)
{
}

function_token::function_token(const function_token& r) :
    formula_token_base(r),
    m_func_oc(r.m_func_oc)
{
}

function_token::~function_token()
{
}

size_t function_token::get_index() const
{
    return m_func_oc;
}

bool operator== (const formula_tokens_t& left, const formula_tokens_t& right)
{
    size_t n = left.size();
    if (n != right.size())
        return false;

    formula_tokens_t::const_iterator itr = left.begin(), itr_end = left.end(), itr2 = right.begin();
    for (; itr != itr_end; ++itr, ++itr2)
    {
        if (*itr != *itr2)
            return false;
    }
    return true;
}

}
/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
