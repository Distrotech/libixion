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

#include "cell.hpp"
#include "formula_interpreter.hpp"
#include "formula_result.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <string>
#include <sstream>
#include <iostream>

#define DEBUG_FORMULA_CELL 0

using namespace std;

namespace ixion {

namespace {

class ref_token_picker : public unary_function<formula_token_base, void>
{
public:
    ref_token_picker() :
        mp_tokens(new vector<formula_token_base*>()) {}

    ref_token_picker(const ref_token_picker& r) :
        mp_tokens(r.mp_tokens) {}

    void operator() (formula_token_base& t)
    {
        if (t.get_opcode() == fop_single_ref)
            mp_tokens->push_back(&t);
    }

    void swap_tokens(vector<formula_token_base*>& dest)
    {
        mp_tokens->swap(dest);
    }

private:
    ::boost::shared_ptr<vector<formula_token_base*> > mp_tokens;
};

}

// ============================================================================

address::address()
{
}

address::~address()
{
}

// ============================================================================

base_cell::base_cell(celltype_t celltype) :
    m_celltype(celltype)
{
}

base_cell::base_cell(const base_cell& r) :
    m_celltype(r.m_celltype)
{
}

base_cell::~base_cell()
{
}

void base_cell::add_listener(formula_cell* p)
{
    m_listeners.insert(p);
}

void base_cell::remove_listener(formula_cell* p)
{
    m_listeners.erase(p);
}

void base_cell::print_listeners() const
{
    cout << "The following cells listen to cell " << global::get_cell_name(this) << endl;
    unordered_set<formula_cell*>::const_iterator itr = m_listeners.begin(), itr_end = m_listeners.end();
    for (; itr != itr_end; ++itr)
    {
        cout << "  cell " << global::get_cell_name(*itr) << endl;
    }
}

void base_cell::get_all_listeners(dirty_cells_t& cells) const
{
    listeners_type::const_iterator itr = m_listeners.begin(), itr_end = m_listeners.end();
    for (; itr != itr_end; ++itr)
    {
        formula_cell* p = *itr;
        if (cells.count(p) == 0)
        {
            // This cell is not yet on the dirty cell list.  Run recursively.
            cells.insert(p);
            p->get_all_listeners(cells);
        }
    }
}

celltype_t base_cell::get_celltype() const
{
    return m_celltype;
}

// ============================================================================

string_cell::string_cell(const string& formula) :
    base_cell(celltype_string),
    m_formula(formula)
{
}

string_cell::string_cell(const string_cell& r) :
    base_cell(r),
    m_formula(r.m_formula)
{
}

string_cell::~string_cell()
{
}

const char* string_cell::print() const
{
    return m_formula.c_str();
}

// ============================================================================

formula_cell::interpret_status::interpret_status() : 
    result(NULL)
{}

formula_cell::interpret_status::interpret_status(const interpret_status& r) :
    result(NULL)
{
    if (r.result)
        result = new formula_result(*r.result);
}

formula_cell::interpret_status::~interpret_status()
{
    delete result;
}

// ============================================================================

formula_cell::formula_cell() :
    base_cell(celltype_formula),
    m_circular_safe(false)
{
}

formula_cell::formula_cell(formula_tokens_t& tokens) :
    base_cell(celltype_formula),
    m_circular_safe(false)
{
    // Note that this will empty the passed token container !
    m_tokens.swap(tokens);
}

formula_cell::formula_cell(const formula_cell& r) :
    base_cell(r),
    m_tokens(r.m_tokens),
    m_interpret_status(r.m_interpret_status),
    m_circular_safe(r.m_circular_safe)
{
}

formula_cell::~formula_cell()
{
}

double formula_cell::get_value() const
{
    ::boost::mutex::scoped_lock lock(m_interpret_status.mtx);
    wait_for_interpreted_result(lock);

    if (!m_interpret_status.result)
        // Result not cached yet.  Reference error.
        throw formula_error(fe_ref_result_not_available);

    if (m_interpret_status.result->get_type() == formula_result::rt_error)
        // Error condition.
        throw formula_error(m_interpret_status.result->get_error());

    assert(m_interpret_status.result->get_type() == formula_result::rt_value);
    return m_interpret_status.result->get_value();
}

const char* formula_cell::print() const
{
    return print_tokens(m_tokens, false);
}

const formula_tokens_t& formula_cell::get_tokens() const
{
    return m_tokens;
}

void formula_cell::interpret(const cell_ptr_name_map_t& cell_ptr_name_map)
{
#if DEBUG_FORMULA_CELL
    ostringstream os;
    os << get_formula_result_output_separator() << endl;
    os << global::get_cell_name(this) << ": interpreting" << endl;
    cout << os.str();
#endif
    {
        ::boost::mutex::scoped_lock lock(m_interpret_status.mtx);

        string cell_name = global::get_cell_name(this);
    
        if (m_interpret_status.result)
        {
            // When the result is already cached before the cell is interpreted, 
            // it can mean the cell has circular dependency.
            if (m_interpret_status.result->get_type() == formula_result::rt_error)
            {
                ostringstream os;
                os << get_formula_result_output_separator() << endl;
                os << cell_name << ": result = " << get_formula_error_name(m_interpret_status.result->get_error()) << endl;
                cout << os.str();
            }
            return;
        }
    
        formula_interpreter fin(this, cell_ptr_name_map);
        m_interpret_status.result = new formula_result;
        if (fin.interpret())
        {
            // Successful interpretation.
            m_interpret_status.result->set_value(fin.get_result());
        }
        else
        {
            // Interpretation ended with an error condition.
            m_interpret_status.result->set_error(fin.get_error());
        }
    }
    m_interpret_status.cond.notify_all();
}

bool formula_cell::is_circular_safe() const
{
    return m_circular_safe;
}

void formula_cell::check_circular()
{
    // TODO: Check to make sure this is being run on the main thread only.

    formula_tokens_t::iterator itr = m_tokens.begin(), itr_end = m_tokens.end();
    for (; itr != itr_end; ++itr)
    {
        fopcode_t op = itr->get_opcode();
        if (op != fop_single_ref)
            continue;

        const base_cell* ref = itr->get_single_ref();
        if (ref->get_celltype() != celltype_formula)
            continue;

        if (!static_cast<const formula_cell*>(ref)->is_circular_safe())
        {
            // Circular dependency detected !!
#if DEBUG_FORMULA_CELL
            ostringstream os;
            os << global::get_cell_name(this) << ": ";
            os << "circular dependency detected !!" << endl;
            cout << os.str();
#endif
            assert(!m_interpret_status.result);
            m_interpret_status.result = new formula_result(fe_ref_result_not_available);
            return;
        }
    }

    // No circular dependencies.  Good.
    m_circular_safe = true;
}

void formula_cell::reset()
{
#if DEBUG_FORMULA_CELL
    ostringstream os;
    os << global::get_cell_name(this) << ": reset" << endl;
    cout << os.str();
#endif
    ::boost::mutex::scoped_lock lock(m_interpret_status.mtx);
    delete m_interpret_status.result;
    m_interpret_status.result = NULL;
    m_circular_safe = false;
}

void formula_cell::swap_tokens(formula_tokens_t& tokens)
{
    m_tokens.swap(tokens);
}

void formula_cell::get_ref_tokens(vector<formula_token_base*>& tokens)
{
    ref_token_picker func;
    for_each(m_tokens.begin(), m_tokens.end(), func).swap_tokens(tokens);
}

const formula_result* formula_cell::get_result_cache() const
{
    ::boost::mutex::scoped_lock lock(m_interpret_status.mtx);
    return m_interpret_status.result;
}

void formula_cell::wait_for_interpreted_result(::boost::mutex::scoped_lock& lock) const
{
    while (!m_interpret_status.result)
    {
#if DEBUG_FORMULA_CELL
        ostringstream os;
        os << global::get_cell_name(this) << ": ";
        os << "waiting" << endl;
        cout << os.str();
#endif
        m_interpret_status.cond.wait(lock);
    }
}

}