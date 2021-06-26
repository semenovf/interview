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
#pragma once
#include <string>
#include <locale>
#include <ctime>
#include <cctype>

/**
 * Skip whitespaces
 */
inline void skipWhitespaces (std::string::const_iterator & pos
    , std::string::const_iterator last)
{
    while (pos != last && std::isspace(*pos, std::locale()))
        ++pos;
}

/**
 * Advance sequence of digits
 */
inline bool advanceDigits (std::string::const_iterator & pos
    , std::string::const_iterator last)
{
    int count = 0;
    for (; pos != last && std::isdigit(*pos, std::locale()); count++, ++pos)
        ;

    return count > 0;
}

/**
 * Advance date according the format (d+ '/' d+)
 */
bool advanceShortDate (std::string::const_iterator & pos
    , std::string::const_iterator last);

/**
 * Advance date and time according the format (YYYY-MM-DD hh:mm:ss)
 */
bool advanceDateTime (std::string::const_iterator & pos
    , std::string::const_iterator last
    , std::tm & t);

/**
 * Advance time according the format (d+ ':' d+ ':' d+ '-' d+)
 */
bool advanceTime (std::string::const_iterator & pos
    , std::string::const_iterator last);

/**
 * Advance first parameter from parameters list delimited by comma.
 */
bool advanceFirstParm (std::string::const_iterator & pos
    , std::string::const_iterator last
    , std::string::const_iterator & parmFirst
    , std::string::const_iterator & parmLast);

/**
 * Advance next parameter from parameters list delimited by comma.
 */
bool advanceNextParm (std::string::const_iterator & pos
    , std::string::const_iterator last
    , std::string::const_iterator & parmFirst
    , std::string::const_iterator & parmLast);
