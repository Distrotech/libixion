/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDED_IXION_INTERFACE_MODEL_CONTEXT_HPP
#define INCLUDED_IXION_INTERFACE_MODEL_CONTEXT_HPP

#include "ixion/formula_tokens.hpp"
#include "ixion/types.hpp"
#include "ixion/exceptions.hpp"

#include <string>
#include <vector>

namespace ixion {

class formula_cell;
class formula_name_resolver;
class cell_listener_tracker;
class matrix;
struct abs_address_t;
struct abs_range_t;
struct config;

namespace iface {

class session_handler;
class table_handler;

/**
 * Interface for allowing access to the model mostly from ixion's formula
 * interpreter and its related classes. The client code needs to provide
 * concrete implementation of this interface in order to provide access to
 * its content.
 */
class formula_model_access
{
    formula_model_access(const formula_model_access&) = delete;
    formula_model_access& operator=(const formula_model_access&) = delete;

public:
    IXION_DLLPUBLIC formula_model_access();
    virtual IXION_DLLPUBLIC ~formula_model_access();

    virtual const config& get_config() const = 0;
    virtual cell_listener_tracker& get_cell_listener_tracker() = 0;

    virtual bool is_empty(const abs_address_t& addr) const = 0;
    virtual celltype_t get_celltype(const abs_address_t& addr) const = 0;

    /**
     * Get a numeric representation of the cell value at specified position.
     * If the cell at the specified position is a formula cell and its result
     * has not yet been computed, it will block until the result becomes
     * available.  Call this only during formula (re-)calculation.
     *
     * @param addr position of the cell.
     *
     * @return numeric representation of the cell value.
     */
    virtual double get_numeric_value(const abs_address_t& addr) const = 0;
    virtual string_id_t get_string_identifier(const abs_address_t& addr) const = 0;
    virtual string_id_t get_string_identifier(const char* p, size_t n) const = 0;
    virtual const formula_cell* get_formula_cell(const abs_address_t& addr) const = 0;
    virtual formula_cell* get_formula_cell(const abs_address_t& addr) = 0;

    virtual const formula_cell* get_named_expression(const ::std::string& name) const = 0;
    virtual const ::std::string* get_named_expression_name(const formula_cell* expr) const = 0;

    virtual double count_range(const abs_range_t& range, const values_t& values_type) const = 0;

    /**
     * Obtain range value in matrix form.  Multi-sheet ranges are not
     * supported.  If the specified range consists of multiple sheets, it
     * throws an exception.
     *
     * @param range absolute, single-sheet range address.  Multi-sheet ranges
     *              are not allowed.
     *
     * @return range value represented as matrix.
     */
    virtual matrix get_range_value(const abs_range_t& range) const = 0;

    /**
     * Session handler instance receives various events from the formula
     * interpretation run, in order to respond to those events.  This is
     * optional; the model context implementation is not required to provide a
     * handler.
     *
     * @return non-NULL pointer of the model context provides a session
     *         handler, otherwise a NULL pointer is returned.
     */
    virtual session_handler* get_session_handler();

    /**
     * Table interface provides access to all table ranges stored in the
     * document model.  A table is a 2-dimensional range of cells that include
     * named columns.  It is used when resolving a table reference that refers
     * to a cell or a range of cells by the table name and/or column name.
     *
     * @return non-NULL pointer to the table storage inside the model, or NULL
     *         if no table is present or supported by the model
     *         implementation.
     */
    virtual table_handler* get_table_handler();

    virtual const table_handler* get_table_handler() const;

    virtual const formula_tokens_t* get_formula_tokens(sheet_t sheet, size_t identifier) const = 0;
    virtual const formula_tokens_t* get_shared_formula_tokens(sheet_t sheet, size_t identifier) const = 0;
    virtual abs_range_t get_shared_formula_range(sheet_t sheet, size_t identifier) const = 0;

    virtual string_id_t append_string(const char* p, size_t n) = 0;
    virtual string_id_t add_string(const char* p, size_t n) = 0;
    virtual const std::string* get_string(string_id_t identifier) const = 0;

    /**
     * Get the index of sheet from sheet name.
     *
     * @param p pointer to the first character of the sheet name string.
     * @param n length of the sheet name string.
     *
     * @return sheet index
     */
    virtual sheet_t get_sheet_index(const char* p, size_t n) const = 0;

    virtual std::string get_sheet_name(sheet_t sheet) const = 0;
};

}}

#endif
/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
