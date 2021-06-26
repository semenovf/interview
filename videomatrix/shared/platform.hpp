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
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <locale>
#include <regex>

#if (defined(_WIN32) || defined(_WIN64)) && !defined(UNICODE)
#   define PLATFORM_WINDOWS_ANSI 1
#endif

#if defined(__linux__)
#   define PLATFORM_LINUX 1
#endif

#if (defined(_WIN32) || defined(_WIN64)) && defined(UNICODE)
#   include <windows.h>
#   include <cwctype>
#   define PLATFORM_WINDOWS_UNICODE 1
#   define LITERAL(s) L##s
#   define PATH_SEPARATOR L'\\'

using string_type = std::wstring;
using istream_type = std::wistream;
using ostream_type = std::wostream;
using ifstream_type = std::wifstream;
using ofstream_type = std::wofstream;
using regex_type = std::wregex;

inline std::wostream & cout ()
{
    return std::wcout;
}

inline std::wostream & cerr ()
{
    return std::wcerr;
}

inline std::wistream & cin ()
{
    return std::wcin;
}

inline bool is_space (wint_t ch)
{
    return std::iswspace(ch) == 0 ? false : true;
}

inline bool is_digit (wint_t ch)
{
    return std::iswdigit(ch) == 0 ? false : true;
}

inline string_type convert(std::string::const_iterator first
    , std::string::const_iterator last)
{
    int utf16len = MultiByteToWideChar(CP_UTF8, 0
        , & *first, std::distance(first, last)
        , NULL, 0);

    string_type result;
    result.resize(utf16len);

    MultiByteToWideChar(CP_UTF8, 0
        , & *first, std::distance(first, last)
        , & result[0], result.size());

    return result;
}

inline string_type convert(std::string const & orig)
{
    return convert(orig.begin(), orig.end());
}

#elif defined(PLATFORM_LINUX) || defined(PLATFORM_WINDOWS_ANSI)
#   include <cctype>

#   define PLATFORM_LINUX 1
#   define LITERAL(s) s
#   define PATH_SEPARATOR '/'

using string_type = std::string;
using istream_type = std::istream;
using ostream_type = std::ostream;
using ifstream_type = std::ifstream;
using ofstream_type = std::ofstream;
using regex_type = std::regex;

inline std::ostream & cout ()
{
    return std::cout;
}

inline std::ostream & cerr ()
{
    return std::cout;
}

inline std::istream & cin ()
{
    return std::cin;
}

inline bool is_space (int ch)
{
    return std::isspace(ch, std::locale{}) == 0 ? false : true;
}

inline bool is_digit (int ch)
{
    return std::isdigit(ch, std::locale{}) == 0 ? false : true;
}

inline string_type const & convert (std::string const & orig)
{
    return orig;
}

inline string_type convert(std::string::const_iterator first
    , std::string::const_iterator last)
{
    return string_type{first, last};
}

#endif
