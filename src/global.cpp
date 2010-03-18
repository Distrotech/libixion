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

#include "global.hpp"

#include <sstream>

using namespace std;

namespace ixion {

namespace {

class token_printer : public unary_function<token_base, void>
{
public:
    token_printer(ostringstream& os, bool verbose) : m_os(os), m_verbose(verbose) {}
    void operator() (const token_base& r) const
    {
        opcode_t oc = r.get_opcode();
        if (m_verbose)
            m_os << "(" << get_opcode_name(oc) << ")'" << r.print() << "' ";
        else
            m_os << r.print();
    }
private:
    ostringstream& m_os;
    bool m_verbose;
};

}

const char* print_tokens(const tokens_t& tokens, bool verbose)
{
    ostringstream os;
    for_each(tokens.begin(), tokens.end(), token_printer(os, verbose));
    return os.str().c_str();
}

}