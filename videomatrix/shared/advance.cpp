////////////////////////////////////////////////////////////////////////////////
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//
// Copyright © 2020 Vladislav Trifochkin <wladt@jobquest.ru>
//
// Permission is hereby  granted, free of charge, to any  person obtaining a copy
// of this software and associated  documentation files (the "Software"), to deal
// in the Software  without restriction, including without  limitation the rights
// to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
// copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
// IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
// FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
// AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
// LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////
#include "advance.hpp"
#include <sstream>
#include <locale>
#include <iomanip>
#include <cstring>

bool advanceShortDate (std::string::const_iterator & pos
    , std::string::const_iterator last)
{
    if (!advanceDigits(pos, last))
        return false;

    // Unexpected end of line
    if (pos == last)
        return false;

    if (*pos != '/')
        return false;

    ++pos;

    if (!advanceDigits(pos, last))
        return false;

    return true;
}

bool advanceDateTime (std::string::const_iterator & pos
    , std::string::const_iterator last
    , std::tm & t)
{
    memset(& t, 0, sizeof(t));

    auto first = pos;

    // Parse year
    if (!advanceDigits(pos, last))
        return false;

    t.tm_year = std::stoi(std::string{ first, pos }) - 1900;

    // Unexpected end of line
    if (pos == last)
        return false;

    if (*pos != '-')
        return false;

    ++pos;

    // Parse month
    first = pos;

    if (!advanceDigits(pos, last))
        return false;

    t.tm_mon = std::stoi(std::string{ first, pos });

    // Unexpected end of line
    if (pos == last)
        return false;

    if (*pos != '-')
        return false;

    ++pos;

    // Parse day
    first = pos;

    if (!advanceDigits(pos, last))
        return false;

    t.tm_mday = std::stoi(std::string{ first, pos });

    skipWhitespaces(pos, last);

    // Unexpected end of line
    if (pos == last)
        return false;

    // Parse hours
    first = pos;

    if (!advanceDigits(pos, last))
        return false;

    t.tm_hour = std::stoi(std::string{ first, pos });

    // Unexpected end of line
    if (pos == last)
        return false;

    if (*pos != ':')
        return false;

    ++pos;

    // Parse minutes
    first = pos;

    if (!advanceDigits(pos, last))
        return false;

    t.tm_min = std::stoi(std::string{ first, pos });

    //std::istringstream ss(std::string{pos, last});
    //ss.imbue(std::locale("C"));
    //ss >> std::get_time(& t, "%Y-%m-%d %H:%M"); //  Cause exception in MSVC 2015: istreambuf_iterator is not dereferencable
    //if (ss.fail())
    //    return false;
    //std::advance(pos, static_cast<int>(ss.tellg()));

    return true;
}


bool advanceTime (std::string::const_iterator & pos
    , std::string::const_iterator last)
{
    if (!advanceDigits(pos, last))
        return false;

    // Unexpected end of line
    if (pos == last)
        return false;

    if (*pos != ':')
        return false;

    ++pos;

    if (!advanceDigits(pos, last))
        return false;

    // Unexpected end of line
    if (pos == last)
        return false;

    if (*pos != ':')
        return false;

    ++pos;

    if (!advanceDigits(pos, last))
        return false;

    // Unexpected end of line
    if (pos == last)
        return false;

    if (*pos != '-')
        return false;

    ++pos;

    if (!advanceDigits(pos, last))
        return false;

    return true;
}

/**
 * Parse parameter from list in format (parameter (, | END-OF-LINE)
 */
bool advanceFirstParm (std::string::const_iterator & pos
    , std::string::const_iterator last
    , std::string::const_iterator & parmFirst
    , std::string::const_iterator & parmLast)
{
    skipWhitespaces(pos, last);

    parmFirst = pos;

    while (!(pos == last || *pos == ','))
        ++pos;

    parmLast = pos;

    return parmFirst != parmLast;
}

bool advanceNextParm (std::string::const_iterator & pos
    , std::string::const_iterator last
    , std::string::const_iterator & parmFirst
    , std::string::const_iterator & parmLast)
{
    if (pos == last)
        return false;

    if (*pos != ',')
        return false;

    ++pos;

    skipWhitespaces(pos, last);

    parmFirst = pos;

    while (!(pos == last || *pos == ','))
        ++pos;

    parmLast = pos;

    return true;
}
