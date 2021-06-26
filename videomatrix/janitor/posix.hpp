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
#include "common.hpp"
#include <cassert>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>

class PosixTraverseTraits
{
    DIR * _dir {nullptr};

public:
    PosixTraverseTraits () {}

    ~PosixTraverseTraits ()
    {
        if (_dir) {
            closedir(_dir);
            _dir = nullptr;
        }
    }

    bool isFolder (string_type const & path) const
    {
        struct stat statbuf;

        int rc = stat(path.c_str(), & statbuf);

        if (rc != 0)
            return false;

        switch (statbuf.st_mode & S_IFMT) {
           case S_IFDIR:
               return true;
           default:
               break;
        }

        return false;
    }

    bool modificationTimeExceeded  (string_type const & path, std::time_t timelimit)
    {
        struct stat statbuf;

        int rc = stat(path.c_str(), & statbuf);

        if (rc != 0)
            return false;

        return (statbuf.st_mtime < timelimit) ? true : false;
    }

    string_type currentFolder () const
    {
        return string_type(::get_current_dir_name());
    }

   /**
    * Change current folder.
    *
    * @return @c true if change folder is successful, @c false otherwise.
    */
    bool changeFolder (string_type const & folder)
    {
        auto rc = ::chdir(folder.c_str());
        return (rc == 0) ? true : false;
    }

    bool removeFolderEntry (string_type const & path)
    {
        int rc = 0;

        if (isFolder(path))
            rc = rmdir(path.c_str());
        else
            rc = unlink(path.c_str());

        return (rc == 0) ? true : false;
    }

    /**
     * Returns first folder entry for initial folder @a initialFolder or empty
     * string if error occurred or no entries found.
     */
    FolderEntry firstEntry (string_type const & initialFolder)
    {
        if (_dir) {
            closedir(_dir);
            _dir = nullptr;
        }

        _dir = opendir(initialFolder.c_str());

        if (_dir == nullptr)
            return FolderEntry{};

        return nextEntry();
    }

    FolderEntry nextEntry ()
    {
        assert(_dir); // Or use exception here

        auto entry = readdir(_dir);

        if (entry == nullptr)
            return FolderEntry{};

        FolderEntry result;

        if (entry->d_type == DT_REG)
            result.type = FolderEntryType::Regular;
        else if (entry->d_type == DT_DIR)
            result.type = FolderEntryType::Folder;
        else
            result.type = FolderEntryType::Other;

        result.name = string_type{entry->d_name};

        return result;
    }
};

// Source:
// https://stackoverflow.com/questions/421860/capture-characters-from-standard-input-without-waiting-for-enter-to-be-pressed
// inline char getch ()
// {
//     char buf = 0;
//     struct termios old = {0};
//
//     if (tcgetattr(0, & old) < 0)
//         perror("tcsetattr()");
//
//     old.c_lflag &= ~ICANON;
//     old.c_lflag &= ~ECHO;
//     old.c_cc[VMIN] = 1;
//     old.c_cc[VTIME] = 0;
//
//     if (tcsetattr(0, TCSANOW, & old) < 0)
//         perror("tcsetattr ICANON");
//
//     if (read(0, &buf, 1) < 0)
//         perror("read()");
//
//     old.c_lflag |= ICANON;
//     old.c_lflag |= ECHO;
//
//     if (tcsetattr(0, TCSADRAIN, & old) < 0)
//         perror("tcsetattr ~ICANON");
//     return (buf);
// }
