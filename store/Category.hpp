#pragma once

#include <memory>
#include <string>
#include <system_error>

class Category
{
    friend class CategoryTree;

public:
    typedef std::shared_ptr<Category> shared_type;

protected:
    Category (std::string const & name)
        : _name(name)
    {}

    void addChild (shared_type const & child)
    {
        _children.push_back(child);
    }

public:
    std::string to_string () const
    {
        std::string result(_name);

        shared_type parent = _parent;

        while (parent) {
            result = parent->_name + "->" + result;
            parent = parent->_parent;
        }

        return result;
    }

private:
    std::string              _name;
    shared_type              _parent;
    std::vector<shared_type> _children;
};

class CategoryTree
{
public:
    CategoryTree ()
        : _root(new Category("root"))
    {}

    ~CategoryTree () = default;

    Category::shared_type make (std::string const & name)
    {
        return make(name, _root);
    }

    Category::shared_type make (std::string const & name
            , Category::shared_type const & parent)
    {
        auto result = Category::shared_type(new Category(name));
        result->_parent = parent;
        parent->addChild(result);
        return result;
    }

    // TODO Implement
    std::error_code loadFromResource ()
    {
        return std::error_code();
    }

private:
    std::shared_ptr<Category> _root;
};