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
#include "common.hpp"
#include "advance.hpp"
#include <istream>
#include <string>
#include <cassert>
#include <cctype>

static constexpr int DEFAILT_TIMEOUT = 10;

//
// Parses configuration items in format:
// wildcard \s+ dir \s+ YYYY-MM-DD \s+ HH:MM
//
ConfigItem parseConfigItem (std::string::const_iterator first
    , std::string::const_iterator last)
{
    auto pos = first;

    skipWhitespaces(pos, last);

    // Unexpected end of line
    if (pos == last)
        return ConfigItem{};

    first = pos;

    while (pos != last && !std::isspace(*pos))
        ++pos;

    ConfigItem result;
    result.wildcard = convert(first, pos);

    skipWhitespaces(pos, last);

    // Unexpected end of line
    if (pos == last)
        return ConfigItem{};

    bool quoted = false;

    if (*pos == '"') {
        ++pos;
        quoted = true;
    }

    // Unexpected end of line
    if (pos == last)
        return ConfigItem{};

    first = pos;

    if (quoted) {
        while (pos != last && *pos != '"')
            ++pos;

        // Quote is not balanced
        if (pos == last)
            return ConfigItem{};

        assert(*pos == '"');

        result.folder = convert(first, pos);
        ++pos;
    } else {
        while (pos != last && !std::isspace(*pos))
            ++pos;

        result.folder = convert(first, pos);
    }

    skipWhitespaces(pos, last);

    // Unexpected end of line
    if (pos == last)
        return ConfigItem{};

    first = pos;

    std::tm modtime;

    if (!advanceDateTime(pos, last, modtime))
        return ConfigItem{};

    result.modtime = std::mktime(& modtime);

    return result;
}

Config loadConfig (std::istream & in)
{
    std::string buffer;
    int timeout {0};

    // Read timeout
    if (std::getline(in, buffer)) {
        timeout = std::stoi(buffer);
    }

    if (timeout < 0)
        timeout = DEFAILT_TIMEOUT;

    Config result{timeout};

    while (std::getline(in, buffer)) {
        auto configItem = parseConfigItem(buffer.cbegin(), buffer.cend());

        if (!isValid(configItem))
            return Config{};

        result.append(std::move(configItem));
    }

    return result;

}
