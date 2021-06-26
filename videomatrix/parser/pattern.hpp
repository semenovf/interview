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
#include "platform.hpp"
#include "advance.hpp"
#include <map>
#include <utility>
#include <vector>

class PatternItem
{
    using const_iterator = string_type::const_iterator;
    using range_type = std::pair<const_iterator, const_iterator>;
    using token_type = std::pair<range_type, int>;

    int _code {-1};
    string_type _text;
    std::vector<token_type> _tokens;

public:
    PatternItem () = default;
    PatternItem (PatternItem const &) = delete;
    PatternItem (PatternItem &&) = default;
    PatternItem & operator = (PatternItem const &) = delete;
    PatternItem & operator = (PatternItem &&) = default;

    PatternItem (int code, string_type && text)
        : _code(code)
        , _text(std::forward<string_type>(text))
    {}

    int code () const
    {
        return _code;
    }

    string_type const & text () const
    {
        return _text;
    }

    bool valid () const
    {
        return _code >= 0;
    }

    void appendToken (const_iterator first, const_iterator last, int code)
    {
        _tokens.push_back(std::make_pair(std::make_pair(first, last), code));
    }

    void appendToken (const_iterator first, const_iterator last)
    {
        appendToken(first, last, -1);
    }

    template <typename TernaryOp>
    void forEachToken (TernaryOp p) const
    {
        for (auto const & token: _tokens) {
            p(token.second, token.first.first, token.first.second);
        }
    }
};

using PatternsMap = std::map<int, PatternItem>;

PatternsMap readPatterns (istream_type & in);
