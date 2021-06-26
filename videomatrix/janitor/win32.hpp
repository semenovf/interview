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

class WindowsTraverseTraits
{
    HANDLE hFind {INVALID_HANDLE_VALUE};

public:
    WindowsTraverseTraits() {}

    ~WindowsTraverseTraits()
    {
        if (hFind != INVALID_HANDLE_VALUE) {
            FindClose(hFind);
            hFind = INVALID_HANDLE_VALUE;
        }
    }

    bool isFolder (string_type const & path) const
    {
        auto attrs = GetFileAttributes(path.c_str());

        if (attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY))
            return true;
        return false;
    }

    bool changeFolder(string_type const & folder)
    {
        auto rc = SetCurrentDirectory(folder.c_str());
        return (rc == TRUE) ? true : false;
    }

    bool modificationTimeExceeded (string_type const & path, std::time_t timelimit)
    {
        struct _stat statbuf;

        int rc = _wstat(path.c_str(), & statbuf);

        if (rc != 0)
            return false;

        return (statbuf.st_mtime < timelimit) ? true : false;
    }

    string_type currentFolder () const
    {
        string_type::value_type cwd[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, cwd);
        return string_type(cwd);
    }

    bool removeFolderEntry(string_type const & path)
    {
        BOOL rc = 0;

        if (isFolder(path))
            rc = RemoveDirectory(path.c_str());
        else
            rc = DeleteFile(path.c_str());

        return (rc == TRUE) ? true : false;
    }

    FolderEntry firstEntry (string_type const & initialFolder)
    {
        if (hFind != INVALID_HANDLE_VALUE) {
            FindClose(hFind);
            hFind = INVALID_HANDLE_VALUE;
        }

        WIN32_FIND_DATA findFileData;
        auto mask{ initialFolder };
        mask += PATH_SEPARATOR;
        mask += LITERAL("*");
        hFind = FindFirstFile(mask.c_str()//(initialFolder + PATH_SEPARATOR + LITERAL("*.*")).c_str()
            , & findFileData);

        if (hFind == INVALID_HANDLE_VALUE)
            return FolderEntry{};

        FolderEntry result;

        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            && !(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DEVICE)
            && !(findFileData.dwFileAttributes & FILE_ATTRIBUTE_VIRTUAL))
            result.type = FolderEntryType::Regular;
        else if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            result.type = FolderEntryType::Folder;
        else
            result.type = FolderEntryType::Other;

        result.name = string_type{ findFileData.cFileName };

        return result;
    }

    FolderEntry nextEntry()
    {
        assert(hFind != INVALID_HANDLE_VALUE); // Or use exception here

        WIN32_FIND_DATA findFileData;
        auto rc = FindNextFile(hFind, & findFileData);

        if (rc == FALSE)
            return FolderEntry{};

        FolderEntry result;

        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                && !(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DEVICE)
                && !(findFileData.dwFileAttributes & FILE_ATTRIBUTE_VIRTUAL))
            result.type = FolderEntryType::Regular;
        else if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            result.type = FolderEntryType::Folder;
        else
            result.type = FolderEntryType::Other;

        result.name = string_type{findFileData.cFileName};

        return result;
    }
};
