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
#include "platform.hpp"
#include "advance.hpp"
#include "pattern.hpp"
#include "proto.hpp"

#if PLATFORM_WINDOWS_UNICODE
int wmain (int argc, wchar_t ** argv)
#elif defined(PLATFORM_LINUX) || defined(PLATFORM_WINDOWS_ANSI)
int main (int argc, char ** argv)
#endif
{
#if PLATFORM_WINDOWS_ANSI
    std::locale cp1251_locale("rus_rus.1251");
    std::locale::global(cp1251_locale);
#endif

    if (argc < 3) {
        cerr() << "Too few arguments\n\n"
            << "Usage:\n\t" << argv[0] << " PATTERN_FILE PROTO_FILE [OUTPUT_FILE]\n";
        return -1;
    }

    ifstream_type patternFileStream {argv[1]};
    ifstream_type protoFileStream {argv[2]};

    if (!patternFileStream.is_open()) {
        cerr() << "Failed to open pattern file: " << argv[1] << "\n";
        return -1;
    }

    if (!protoFileStream.is_open()) {
        cerr() << "Failed to open pattern file: " << argv[2] << "\n";
        return -1;
    }

    auto patternsMap = readPatterns(patternFileStream);

    if (patternsMap.empty()) {
        cerr() << "Bad pattern file or no valid patterns found\n";
        return -1;
    }

    if (argc > 3) {
        ofstream_type outStream{argv[3], ofstream_type::out | ofstream_type::app };

        if (!outStream.is_open()) {
            cerr() << "Failed to open output file: " << argv[3] << "\n";
            return -1;
        }

        processProto(protoFileStream, outStream, patternsMap);
        outStream.close();
    } else {
        processProto(protoFileStream, cout(), patternsMap);
    }

    return 0;
}
