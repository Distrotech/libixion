/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef __IXION_ADDRESS_HPP__
#define __IXION_ADDRESS_HPP__

#include "ixion/global.hpp"

#include <string>
#include <vector>
#include <ostream>
#include <unordered_set>

namespace ixion {

/**
 * Row address not specified. This is used to reference an entire column
 * when a specific column address is given.
 */
IXION_DLLPUBLIC_VAR const row_t row_unset;

/**
 * Highest number that can be used to reference a row address. Numbers
 * higher than this number are all used as special indices.
 */
IXION_DLLPUBLIC_VAR const row_t row_upper_bound;

/**
 * Column address not specified. This is used to reference an entire row
 * when a specific row address is given.
 */
IXION_DLLPUBLIC_VAR const col_t column_unset;

/**
 * Highest number that can be used to reference a column address. Numbers
 * higher than this number are all used as special indices.
 */
IXION_DLLPUBLIC_VAR const col_t column_upper_bound;

/**
 * Stores absolute address, and absolute address only.
 */
struct IXION_DLLPUBLIC abs_address_t
{
    enum init_invalid { invalid };

    sheet_t sheet;
    row_t   row;
    col_t   column;

    abs_address_t();
    abs_address_t(init_invalid);
    abs_address_t(sheet_t _sheet, row_t _row, col_t _column);
    abs_address_t(const abs_address_t& r);

    bool valid() const;
    ::std::string get_name() const;

    struct hash
    {
        IXION_DLLPUBLIC size_t operator() (const abs_address_t& addr) const;
    };
};

IXION_DLLPUBLIC bool operator==(const abs_address_t& left, const abs_address_t& right);
IXION_DLLPUBLIC bool operator!=(const abs_address_t& left, const abs_address_t& right);
IXION_DLLPUBLIC bool operator<(const abs_address_t& left, const abs_address_t& right);

/**
 * Stores either absolute or relative address.
 */
struct IXION_DLLPUBLIC address_t
{
    sheet_t sheet;
    row_t   row;
    col_t   column;
    bool    abs_sheet:1;
    bool    abs_row:1;
    bool    abs_column:1;

    address_t();
    address_t(sheet_t _sheet, row_t _row, col_t _column,
              bool _abs_sheet=true, bool _abs_row=true, bool _abs_column=true);
    address_t(const address_t& r);
    address_t(const abs_address_t& r);

    bool valid() const;
    abs_address_t to_abs(const abs_address_t& origin) const;
    ::std::string get_name() const;

    void set_absolute(bool abs);

    struct hash
    {
        size_t operator() (const address_t& addr) const;
    };
};

IXION_DLLPUBLIC bool operator==(const address_t& left, const address_t& right);
IXION_DLLPUBLIC bool operator!=(const address_t& left, const address_t& right);
IXION_DLLPUBLIC bool operator<(const address_t& left, const address_t& right);

/**
 * Stores absolute range address.
 */
struct IXION_DLLPUBLIC abs_range_t
{
    enum init_invalid { invalid };

    abs_address_t first;
    abs_address_t last;

    abs_range_t();
    abs_range_t(init_invalid);

    struct hash
    {
        size_t operator() (const abs_range_t& range) const;
    };

    bool valid() const;
    void set_whole_column();
    void set_whole_row();
    bool whole_column() const;
    bool whole_row() const;

    /**
     * Check whether or not a given address is contained within this range.
     */
    bool contains(const abs_address_t& addr) const;
};

IXION_DLLPUBLIC bool operator==(const abs_range_t& left, const abs_range_t& right);
IXION_DLLPUBLIC bool operator!=(const abs_range_t& left, const abs_range_t& right);
IXION_DLLPUBLIC bool operator<(const abs_range_t& left, const abs_range_t& right);

/**
 * Stores range whose component may be relative or absolute.
 */
struct IXION_DLLPUBLIC range_t
{
    address_t first;
    address_t last;

    range_t();
    range_t(const address_t& _first, const address_t& _last);
    range_t(const range_t& r);
    range_t(const abs_range_t& r);

    bool valid() const;
    void set_whole_column();
    void set_whole_row();
    bool whole_column() const;
    bool whole_row() const;

    abs_range_t to_abs(const abs_address_t& origin) const;
};

IXION_DLLPUBLIC bool operator==(const range_t& left, const range_t& right);
IXION_DLLPUBLIC bool operator!=(const range_t& left, const range_t& right);

IXION_DLLPUBLIC std::ostream& operator<<(std::ostream& os, const abs_address_t& addr);
IXION_DLLPUBLIC std::ostream& operator<<(std::ostream& os, const address_t& addr);
IXION_DLLPUBLIC std::ostream& operator<<(std::ostream& os, const abs_range_t& range);
IXION_DLLPUBLIC std::ostream& operator<<(std::ostream& os, const range_t& range);

/**
 * Collection of formula cells that have been modified or formula cells that
 * reference other modified cells either directly or indirectly.
 */
typedef std::unordered_set<abs_address_t, abs_address_t::hash> dirty_formula_cells_t;

/**
 * Collection of cells that have been modified.
 */
typedef std::vector<abs_address_t> modified_cells_t;

}

#endif
/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
