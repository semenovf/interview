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
#include "pattern.hpp"

static PatternItem parsePatternLine (string_type::const_iterator first
    , string_type::const_iterator last)
{
    if (first == last)
        return PatternItem{};

    auto pos = first;

    if (!advanceDigits(pos, last))
        return PatternItem{};

    auto code = std::stoi(string_type{first, pos});

    // Unexpected end of line
    if (pos == last)
        return PatternItem{};

    skipWhitespaces(pos, last);

    // Unexpected end of line
    if (pos == last)
        return PatternItem{};

    // Expected sign symbol
    if (*pos != LITERAL('='))
        return PatternItem{};

    ++pos;

    skipWhitespaces(pos, last);

    // Unexpected end of line
    if (pos == last)
        return PatternItem{};

    PatternItem pitem{code, string_type{pos, last}};

    // Save first position for text message
    first = pitem.text().begin();
    last = pitem.text().end();
    pos = first;

    while (pos != last) {
        if (*pos == '%') {
            auto placeholderSignPos = pos;
            ++pos;
            auto placeholderFirst = pos;

            if (advanceDigits(pos, last)) {
                pitem.appendToken(first, placeholderSignPos);

                auto placeholderIndex = std::stoi(string_type{placeholderFirst, pos});
                pitem.appendToken(placeholderSignPos, pos, placeholderIndex - 1);
            }

            first = pos;
        } else {
            ++pos;
        }
    }

    if (first != last)
        pitem.appendToken(first, pos);

    return pitem;
}

PatternsMap readPatterns (istream_type & in)
{
    PatternsMap result;
    string_type buffer;

    while (std::getline(in, buffer)) {
        auto pitem = parsePatternLine(buffer.cbegin(), buffer.cend());

        if (!pitem.valid())
            continue;

        result.insert(std::make_pair(pitem.code(), std::move(pitem)));
    }

    return result;
}
