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
#include <thread>
#include <atomic>
#include <cstdio>
#include <ctime>

#if PLATFORM_WINDOWS_UNICODE
#   include "win32.hpp"
#   include <conio.h> // for _kbhit()
#   include <io.h>    // for _setmode()
#   include <fcntl.h> // for _O_U16TEXT
    using TraverseTraits = WindowsTraverseTraits;

#pragma warning(push)
#pragma warning(disable: 4996)
    string_type basename (string_type::pointer path)
    {
        string_type::value_type fname[_MAX_FNAME];
        string_type::value_type ext[_MAX_EXT];

        _wsplitpath(path, nullptr, nullptr, fname, ext);

        string_type result{fname};
        result += ext;

        return result;
    }
#pragma warning(pop)

#elif defined(PLATFORM_LINUX)
#   include "posix.hpp"
#   include <libgen.h> // for basename()
#   include <termios.h>
#   include <sys/ioctl.h>

    using TraverseTraits = PosixTraverseTraits;

    // see https://www.flipcode.com/archives/_kbhit_for_Linux.shtml
    int _kbhit ()
    {
        static const int STDIN = 0;
        static bool initialized = false;

        if (! initialized) {
            // Use termios to turn off line buffering
            termios term;
            tcgetattr(STDIN, & term);
            term.c_lflag &= ~ICANON;
            tcsetattr(STDIN, TCSANOW, &term);
            setbuf(stdin, NULL);
            initialized = true;
        }

        int bytesWaiting;
        ioctl(STDIN, FIONREAD, & bytesWaiting);
        return bytesWaiting;
    }

#endif

static std::atomic_bool quitFlag{false};

bool isInterrupted ()
{
    return quitFlag == true;
}

void stdout_printer (FolderEntry const & folderEntry)
{
    cout() << "[" << toString(folderEntry.type) << "] "
        << folderEntry.path << "\n";
}

class BaseFilter
{
protected:
    regex_type _pattern;
    std::time_t _modtime;
    TraverseTraits _traits;
    size_t _counter {0};

protected:
    BaseFilter () = default;
    BaseFilter (string_type const & pattern, std::time_t modtime)
        : _pattern(pattern)
        , _modtime(modtime)
    {}

public:
    size_t count () const
    {
        return _counter;
    }
};

class PrinterFilter: public BaseFilter
{
public:
    PrinterFilter (string_type const & pattern, std::time_t modtime)
        : BaseFilter(pattern, modtime)
    {}

    void operator () (FolderEntry const & folderEntry)
    {
        if (std::regex_match(folderEntry.name, _pattern)
                && _traits.modificationTimeExceeded(folderEntry.path, _modtime)) {
            stdout_printer(folderEntry);
            ++_counter;
        }
    }
};

class DeleterFilter: public BaseFilter
{
public:
    DeleterFilter (string_type const & pattern, std::time_t modtime)
        : BaseFilter(pattern, modtime)
    {}

    void operator () (FolderEntry const & folderEntry)
    {
        if (std::regex_match(folderEntry.name, _pattern)
                && _traits.modificationTimeExceeded(folderEntry.path, _modtime)) {

            if (_traits.isFolder(folderEntry.path)) {
                recursiveRemoveFolder<TraverseTraits>(folderEntry.path, isInterrupted);
                _traits.removeFolderEntry(folderEntry.path);
            } else {
                _traits.removeFolderEntry(folderEntry.path);
            }

            ++_counter;
        }
    }

};

#if PLATFORM_WINDOWS_UNICODE
int wmain(int argc, wchar_t ** argv)
{
    _setmode(_fileno(stdout), _O_U16TEXT);
#elif defined(PLATFORM_LINUX) || defined(PLATFORM_WINDOWS_ANSI)
int main(int argc, char ** argv)
{
#endif
    string_type configPath;
    bool doNotDelete = false;

    if (argc < 2) {
        cerr() << "Too few arguments\n\n"
            << "Usage: " << basename(argv[0]) << " [-n] CONFIG_FILE\n"
            << "\t-n do not actually delete files (only print to stdout)\n";
        return -1;
    }

    if (argc > 2) {
        string_type opt {argv[1]};

        if (opt == LITERAL("-n"))
            doNotDelete = true;

        configPath = argv[2];
    } else {
        configPath = argv[1];
    }

    cout() << "Press any key to exit\n\n";

    std::thread userInput{[] {
        while (!quitFlag && !_kbhit()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        quitFlag = true;
    }};

    auto workingFolder = TraverseTraits().currentFolder();

    while (!quitFlag) {
        TraverseTraits().changeFolder(workingFolder);
        std::ifstream configFileStream {configPath};

        cout() << "Loading configuration from file: " << configPath << "\n";

        if (!configFileStream.is_open()) {
            cerr() << "Failed to open configuration file: " << configPath << "\n";
            break;
        }

        auto config = loadConfig(configFileStream);

        configFileStream.close();

        if (config.empty()) {
            cerr() << "Bad configuration file or no valid items found\n";
            break;
        }

        if (doNotDelete) {
            config.forEachItem([] (ConfigItem const & item) {
                PrinterFilter filter{convertToRegexPattern(item.wildcard), item.modtime};
                recursiveTraverseFolder<TraverseTraits>(item.folder
                    , isInterrupted
                    , std::ref(filter));

                cout() << "Total entries found in '" << item.folder
                    << "': " << filter.count() << "\n";
            });
        } else {
            config.forEachItem([] (ConfigItem const & item) {
                DeleterFilter filter{convertToRegexPattern(item.wildcard), item.modtime};

                recursiveTraverseFolder<TraverseTraits>(item.folder
                    , isInterrupted
                    , std::ref(filter));

                cout() << "Total entries removed in '" << item.folder
                    << "': " << filter.count() << "\n";
            });
        }

        if (!quitFlag) {
            auto timeout = config.timeout();
            cout() << "Sleep for " << timeout << " seconds\n";

            while (!quitFlag && timeout--) {
                cout() << "Remains " << timeout << " seconds\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
    }

    quitFlag = true;
    userInput.join();

    return 0;
}
