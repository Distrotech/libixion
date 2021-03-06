/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDED_FORMULA_TOKENS_HPP
#define INCLUDED_FORMULA_TOKENS_HPP

#include "ixion/address.hpp"
#include "ixion/table.hpp"
#include "ixion/formula_opcode.hpp"

#include <string>
#include <vector>

namespace ixion {

/**
 * Get a printable name for a formula opcode.
 *
 * @param oc formula opcode
 *
 * @return printable name for a formula opcode.
 */
const char* get_opcode_name(fopcode_t oc);

IXION_DLLPUBLIC const char* get_formula_opcode_string(fopcode_t oc);

// ============================================================================

class IXION_DLLPUBLIC formula_token_base
{
public:
    formula_token_base(fopcode_t op);
    formula_token_base(const formula_token_base& r);
    virtual ~formula_token_base() = 0;

    fopcode_t get_opcode() const;

    bool operator== (const formula_token_base& r) const;
    bool operator!= (const formula_token_base& r) const;

    virtual address_t get_single_ref() const;
    virtual range_t get_range_ref() const;
    virtual table_t get_table_ref() const;
    virtual double get_value() const;
    virtual size_t get_index() const;
    virtual std::string get_name() const;

private:
    formula_token_base() = delete;

    fopcode_t m_opcode;
};

typedef std::vector<std::unique_ptr<formula_token_base>> formula_tokens_t;

// ============================================================================

/**
 * Very simple token that stores opcode only.
 */
class opcode_token : public formula_token_base
{
public:
    explicit opcode_token(fopcode_t oc);
    opcode_token(const opcode_token& r);
    virtual ~opcode_token();
};

// ============================================================================

class value_token : public formula_token_base
{
public:
    explicit value_token(double value);
    virtual ~value_token();

    virtual double get_value() const;
private:
    double m_value;
};

class string_token : public formula_token_base
{
    string_token() = delete;
public:
    explicit string_token(size_t str_identifier);
    virtual ~string_token();

    virtual size_t get_index() const;
private:
    size_t m_str_identifier;
};

// ============================================================================

/**
 * Token that stores a cell reference.  Note that the address it stores may
 * be either relative to the origin cell or absolute.
 */
class single_ref_token : public formula_token_base
{
public:
    single_ref_token(const address_t& addr);
    single_ref_token(const single_ref_token& r);
    virtual ~single_ref_token();

    virtual address_t get_single_ref() const;

private:
    address_t m_address;
};

// ============================================================================

class range_ref_token : public formula_token_base
{
public:
    range_ref_token(const range_t& range);
    range_ref_token(const range_ref_token& r);
    virtual ~range_ref_token();

    virtual range_t get_range_ref() const;

private:
    range_t m_range;
};

class table_ref_token : public formula_token_base
{
public:
    table_ref_token(const table_t& table);
    table_ref_token(const table_ref_token& r);
    virtual ~table_ref_token();

    virtual table_t get_table_ref() const;

private:
    table_t m_table;
};

// ============================================================================

/**
 * Token that stores a named expression.
 */
class named_exp_token : public formula_token_base
{
public:
    named_exp_token(const char* p, size_t n);
    named_exp_token(const named_exp_token& r);
    virtual ~named_exp_token();
    virtual std::string get_name() const;
private:
    ::std::string m_name;
};

// ============================================================================

class function_token : public formula_token_base
{
public:
    function_token(size_t func_oc);
    function_token(const function_token& r);
    virtual ~function_token();

    virtual size_t get_index() const;

private:
    size_t m_func_oc;
};

// ============================================================================

bool operator== (const formula_tokens_t& left, const formula_tokens_t& right);

}

#endif
/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
