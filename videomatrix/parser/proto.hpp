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
#include "pattern.hpp"
#include <vector>

class ProtoItem
{
    int _code {-1};
    string_type _time;
    std::vector<string_type> _parms;

public:
    ProtoItem () = default;
    ProtoItem (ProtoItem const &) = delete;
    ProtoItem (ProtoItem &&) = default;
    ProtoItem & operator = (ProtoItem const &) = delete;
    ProtoItem & operator = (ProtoItem &&) = default;

    ProtoItem (int code, string_type && time)
        : _code(code)
        , _time(std::forward<string_type>(time))
    {}

    bool valid () const
    {
        return _code >= 0;
    }

    int code () const
    {
        return _code;
    }

    string_type const & time () const
    {
        return _time;
    }

    void append (string_type && parm)
    {
        _parms.push_back(std::forward<string_type>(parm));
    }

    auto size () const -> decltype(_parms.size())
    {
        return _parms.size();
    }

    bool hasParmAt (int index) const
    {
        return (index >= 0 && index < static_cast<int>(_parms.size()));
    }

    string_type const & at (int index) const
    {
        return _parms.at(index);
    }
};

void processProto (istream_type & in
    , ostream_type & out
    , PatternsMap const & patternsMap);
