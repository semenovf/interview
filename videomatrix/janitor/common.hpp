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
#include <stack>
#include <vector>
#include <ctime>

//
// Assume we have no std::filesystem (C++17)
// Will use platform specific filesystem routines.
// Also will not use PIMPL
//

struct ConfigItem
{
    string_type wildcard;
    string_type folder;
    std::time_t modtime;
};

class Config
{
    int _timeout {0};
    std::vector<ConfigItem> _items;

public:
    Config () = default;
    Config (int timeout) : _timeout(timeout)
    {}

    void append (ConfigItem && item)
    {
        _items.push_back(std::forward<ConfigItem>(item));
    }

    bool empty () const
    {
        return _items.empty();
    }

    int timeout () const
    {
        return _timeout;
    }

    template <typename UnaryOp>
    void forEachItem (UnaryOp p) const
    {
        for (auto const & item: _items) {
            p(item);
        }
    }
};

inline bool isValid (ConfigItem const & configItem)
{
    return !configItem.wildcard.empty() && !configItem.folder.empty();
}

Config loadConfig (std::istream & in);

// Consider only directories and regular files
enum class FolderEntryType
{
    Regular, Folder, Other
};

struct FolderEntry
{
    FolderEntryType type;
    string_type path;
    string_type name;
};

inline bool isCurrentOrParentFolder (FolderEntry const & folderEntry)
{
    return (folderEntry.name == LITERAL(".") || folderEntry.name == LITERAL(".."));
}

/**
 * Replaces:
 *      @arg '*' with ".*"
 *      @arg '.' with "\\."
 */
inline string_type convertToRegexPattern (string_type const & wildcard)
{
    string_type result;

    for (auto ch: wildcard) {
        if (ch == LITERAL('*')) {
            result.push_back(LITERAL('.'));
            result.push_back(LITERAL('*'));
        } else if (ch == LITERAL('.')) {
            result.push_back(LITERAL('\\'));
            result.push_back(LITERAL('.'));
        } else {
            result.push_back(ch);
        }
    }

    result.push_back(LITERAL('$'));
    return result;
}

/**
 * Convert FolderEntryType into string.
 */
inline string_type toString (FolderEntryType type)
{
    switch (type) {
        case FolderEntryType::Regular:
            return string_type{LITERAL("R")};
        case FolderEntryType::Folder:
            return string_type{LITERAL("D")};
        default:
            break;
    }
    return string_type{LITERAL("-")};
}

/**
 * Checks if folder entry is valid.
 */
inline bool isValid (FolderEntry const & f)
{
    return !f.name.empty();
}

//
// NOTE: There is ftw() and nftw() in POSIX, but will implement naive analog.
//
template <typename TraverseTraits, typename InterruptObserver, typename UnaryOp>
bool traverseFolder (string_type const & initialFolder
    , InterruptObserver interrupted
    , UnaryOp handler, bool recursive)
{
    TraverseTraits traits;

    if (!traits.changeFolder(initialFolder)) {
        cerr() << "Change folder failed: " << initialFolder << "\n";
        return false;
    }

    std::stack<string_type> folderStack;
    folderStack.push(string_type{LITERAL(".")});

    while (!interrupted() && !folderStack.empty()) {
        auto folderName = folderStack.top();
        auto folderEntry = traits.firstEntry(folderName);

        folderStack.pop();

        while (!interrupted() && isValid(folderEntry)) {
            // Ignore current and parent folders
            if (!(folderEntry.name == LITERAL(".") || folderEntry.name == LITERAL(".."))) {
                folderEntry.path = folderName + PATH_SEPARATOR + folderEntry.name;

                handler(folderEntry);

                if (folderEntry.type == FolderEntryType::Folder) {
                    if (recursive)
                        folderStack.push(folderEntry.path);
                }
            }

            folderEntry = traits.nextEntry();
        }
    }

    return true;
}


/**
 * Do a post-order traversal, that is, call handler() for the directory
 * itself after handling the contents of the directory and its subdirectories.
 */
template <typename TraverseTraits, typename InterruptObserver, typename UnaryOp>
bool traverseFolderDepth (string_type const & folderPath
    , InterruptObserver interrupted
    , UnaryOp handler)
{
    TraverseTraits traits;

    auto folderEntry = traits.firstEntry(folderPath);

    while (!interrupted() && isValid(folderEntry)) {
        // Ignore current and parent folders
        if (!isCurrentOrParentFolder(folderEntry)) {
            folderEntry.path = folderPath + PATH_SEPARATOR + folderEntry.name;

            if (folderEntry.type == FolderEntryType::Folder) {
                traverseFolderDepth<TraverseTraits>(folderEntry.path
                    , interrupted
                    , handler);
            }

            handler(folderEntry);
        }

        folderEntry = traits.nextEntry();
    }

    return true;
}

template <typename TraverseTraits, typename InterruptObserver, typename UnaryOp>
inline bool traverseFolder (string_type const & initialFolder
    , InterruptObserver interrupted
    , UnaryOp handler)
{
    return traverseFolder<TraverseTraits, InterruptObserver, UnaryOp>(
        initialFolder, interrupted, handler, false);
}

template <typename TraverseTraits, typename InterruptObserver, typename UnaryOp>
inline bool recursiveTraverseFolder (string_type const & initialFolder
    , InterruptObserver interrupted
    , UnaryOp handler)
{
    return traverseFolder<TraverseTraits, InterruptObserver, UnaryOp>(
        initialFolder, interrupted, handler, true);
}

template <typename TraverseTraits, typename InterruptObserver>
void recursiveRemoveFolder (string_type const & folderPath
    , InterruptObserver interrupted)
{
    TraverseTraits traits;

    // First clear all folders
    traverseFolderDepth<TraverseTraits>(folderPath
        , interrupted
        , [& traits] (FolderEntry const & folderEntry)
        {
            traits.removeFolderEntry(folderEntry.path);
        });
}
