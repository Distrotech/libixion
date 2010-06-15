/*************************************************************************
 *
 * Copyright (c) 2010 Kohei Yoshida
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 ************************************************************************/

#include "formula_tokens.hpp"

namespace ixion {

const char* print_tokens(const formula_tokens_t& tokens, bool verbose)
{
    return "";
}

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

const base_cell* formula_token_base::get_single_ref() const
{
    return NULL;
}

double formula_token_base::get_value() const
{
    return 0.0;
}

size_t formula_token_base::get_index() const
{
    return 0;
}

// ============================================================================

opcode_token::opcode_token(fopcode_t oc) :
    formula_token_base(oc)
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

// ============================================================================

single_ref_token::single_ref_token(const base_cell* pcell) :
    formula_token_base(fop_single_ref),
    mp_cell(pcell)
{
}

single_ref_token::single_ref_token(const single_ref_token& r) :
    formula_token_base(r),
    mp_cell(r.mp_cell)
{
}

single_ref_token::~single_ref_token()
{
}

const base_cell* single_ref_token::get_single_ref() const
{
    return mp_cell;
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

}