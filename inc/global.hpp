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

#ifndef __IXION_GLOBAL_HPP__
#define __IXION_GLOBAL_HPP__

#include <string>
#include <unordered_map>
#include <boost/ptr_container/ptr_map.hpp>

namespace ixion {

class base_cell;

typedef ::boost::ptr_map< ::std::string, base_cell>             cell_name_ptr_map_t;
typedef ::std::unordered_map<const base_cell*, ::std::string>   cell_ptr_name_map_t;

const char* get_formula_result_output_separator();

::std::string get_cell_name(const cell_ptr_name_map_t& names, const base_cell* cell);

double get_current_time();

// ============================================================================

class general_error : public ::std::exception
{
public:
    explicit general_error(const ::std::string& msg);
    ~general_error() throw();
    virtual const char* what() const throw();
private:
    ::std::string m_msg;
};

// ============================================================================

enum formula_error_t
{
    fe_no_error = 0,
    fe_ref_result_not_available,
    fe_division_by_zero,
    fe_invalid_expression
};

const char* get_formula_error_name(formula_error_t fe);

// ============================================================================

class formula_error : public ::std::exception
{
public:
    explicit formula_error(formula_error_t fe);
    ~formula_error() throw();
    virtual const char* what() const throw();

    formula_error_t get_error() const;
private:
    formula_error_t m_ferror;
};

// ============================================================================

/**
 * Store formula result which may be either numeric or textural.  In case
 * the result is textural, it owns the instance of the string.
 */
class formula_result
{
public:
    enum result_type { rt_value, rt_string, rt_error };

    formula_result();
    formula_result(const formula_result& r);
    formula_result(double v);
    formula_result(::std::string* p);
    formula_result(formula_error_t e);
    ~formula_result();

    void set_value(double v);
    void set_string(::std::string* p);
    void set_error(formula_error_t e);
    
    double get_value() const;
    const ::std::string& get_string() const;
    formula_error_t get_error() const;

    result_type get_type() const;

    ::std::string str() const;

    /**
     * Parse a textural representation of a formula result, and set result 
     * value of appropriate type.
     * 
     * @param str textural representation of a formula result.
     */
    void parse(const ::std::string& str);

    formula_result& operator= (const formula_result& r);

private:
    void parse_error(const char* p);
    void parse_string(const char* p);

private:
    result_type m_type;
    union {
        ::std::string* m_string;
        double m_value;
        formula_error_t m_error;
    };
};

}

#endif
