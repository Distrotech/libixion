/*************************************************************************
 *
 * Copyright (c) 2011 Kohei Yoshida
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

#ifndef __IXION_EXCEPTIONS_HPP__
#define __IXION_EXCEPTIONS_HPP__

#include "env.hpp"

#include <exception>
#include <string>

namespace ixion {

class IXION_DLLPUBLIC general_error : public std::exception
{
public:
    explicit general_error(const std::string& msg);
    ~general_error() throw();
    virtual const char* what() const throw();
private:
    std::string m_msg;
};

class IXION_DLLPUBLIC file_not_found : public std::exception
{
public:
    explicit file_not_found(const std::string& fpath);
    ~file_not_found() throw();
    virtual const char* what() const throw();
private:
    std::string m_fpath;
};

}

#endif
