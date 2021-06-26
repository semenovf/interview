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
#include "proto.hpp"
#include "advance.hpp"
#include <utility>

// Implemented according to adhere to restriction of allocation memory.
static ostream_type & output_range (ostream_type & out
    , string_type::const_iterator first
    , string_type::const_iterator last)
{
    while (first != last) {
        out << *first;
        ++first;
    }

    return out;
}

static ProtoItem parseProtoLine (string_type::const_iterator first
    , string_type::const_iterator last)
{
    if (first == last)
        return ProtoItem{};

    auto pos = first;

    if (!advanceShortDate(pos, last))
        return ProtoItem{};

    skipWhitespaces(pos, last);

    // Unexpected end of line
    if (pos == last)
        return ProtoItem{};

    first = pos;

    if (!advanceTime(pos, last))
        return ProtoItem{};

    string_type time{first, pos};

    skipWhitespaces(pos, last);

    // Unexpected end of line
    if (pos == last)
        return ProtoItem{};

    first = pos;

    if (!advanceDigits(pos, last))
        return ProtoItem{};

    auto code = std::stoi(string_type{first, pos});

    skipWhitespaces(pos, last);

    ProtoItem pitem{code, std::move(time)};

    string_type::const_iterator parmFirst;
    string_type::const_iterator parmLast;

    if (advanceFirstParm(pos, last, parmFirst, parmLast)) {
        pitem.append(string_type{parmFirst, parmLast});

        while (advanceNextParm(pos, last, parmFirst, parmLast)) {
            pitem.append(string_type{parmFirst, parmLast});
        }
    }

    return pitem;
}

void processProto (istream_type & in
    , ostream_type & out
    , PatternsMap const & patternsMap)
{
    string_type buffer;

    while (std::getline(in, buffer)) {
        auto protoItem = parseProtoLine(buffer.cbegin(), buffer.cend());

        // Ignore bad proto line
        if (!protoItem.valid())
            continue;

        auto patternIt = patternsMap.find(protoItem.code());

        if (patternIt == patternsMap.end()) {
            // Error: no pattern by specified code found
            continue;
        }

        patternIt->second.forEachToken([& out, & protoItem] (int code
            , string_type::const_iterator first
            , string_type::const_iterator last)
            {
                out << protoItem.time() << " ";

                if (code >= 0) {
                    if (protoItem.hasParmAt(code)) {
                        out << protoItem.at(code);
                    } else {
                        // Cause exception in MSVC 2015: "Expression: String iterator not dereferencable"
                        //output_range(out, first, last);
                        out << string_type{first, last};
                    }
                } else {
                    // Cause exception in MSVC 2015: "Expression: String iterator not dereferencable"
                    //output_range(out, first, last);
                    out << string_type{first, last};
                }
            });

        out << "\n";
    }
}
