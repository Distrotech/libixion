/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ixion/matrix.hpp"

namespace ixion {

matrix::matrix(size_t rows, size_t cols) :
    m_data(rows, cols) {}

matrix::matrix(const matrix& other) :
    m_data(other.m_data) {}

matrix::~matrix() {}

bool matrix::is_numeric(size_t row, size_t col) const
{
    switch (m_data.get_type(row, col))
    {
        case mdds::mtm::element_numeric:
        case mdds::mtm::element_boolean:
            return true;
        default:
            ;
    }

    return false;
}

double matrix::get_numeric(size_t row, size_t col) const
{
    return m_data.get_numeric(row, col);
}

void matrix::set(size_t row, size_t col, double val)
{
    m_data.set(row, col, val);
}

matrix::size_pair_type matrix::size() const
{
    return m_data.size();
}

}
/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
