#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <map>
#include <numeric>
#include <vector>
#include <string>
#include "Category.hpp"

namespace general {

struct weight
{
    double value;
    
    weight (weight const & w) : value(w.value) {}
    weight (double v) : value(v) {}
};

struct quantity
{
    unsigned int value;
    
    quantity (quantity const & q) : value(q.value) {}
    quantity (unsigned int v) : value(v) {}
};

struct price
{
    double value;
    
    price (price const & p) : value(p.value) {}
    price (double v) : value(v) {}
};

struct cost
{
    double value;
    
    cost (cost const & c) : value(c.value) {}
    cost (double v) : value(v) {}
    
    cost & operator = (cost const & c)
    {
        value = c.value;
        return *this;
    }
    
    bool operator == (cost const & c)
    {
        return value == c.value;
    }
};

inline cost operator + (cost const & lhs, cost const & rhs)
{
    return cost(lhs.value + rhs.value);
}

std::ostream & operator << (std::ostream & out, cost const & c)
{
    out << c.value;
    return out;
}

enum class color {
          UNSPEC
        , RED
        , ORANGE
        , GREEN
};

} // general

template <typename T>
std::string to_string (T const &);

template <>
inline std::string to_string<general::color> (general::color const & color)
{
    switch (color) {
    case general::color::RED: return std::string("red");
    case general::color::ORANGE: return std::string("orange");
    case general::color::GREEN: return std::string("green");
    default: break;
    }
    return std::string("unspecified");
}
//
// Перегрузка функций
//
inline general::cost calculate (general::price const & price
        , general::weight const & weight)
{
    return general::cost(price.value * weight.value);
}

inline general::cost calculate (general::price const & price
        , general::quantity const & quantity)
{
    return general::cost(price.value * quantity.value);
}

class Weighable
{
    general::weight _weight;

public:
    Weighable (general::weight const & weight)
       : _weight(weight)
    {}
       
    general::weight const & weight () const
    {
        return _weight;
    }
};

class Enumerable
{
    general::quantity _quantity;
    
public:
    Enumerable (general::quantity const & quantity)
       : _quantity(quantity)
    {}
       
    general::quantity const & quantity () const
    {
        return _quantity;
    }
};


template <>
std::string to_string<> (Category const & c)
{
    return c.to_string();
}

template <>
std::string to_string<> (Category::shared_type const & c)
{
    return c->to_string();
}

class Product
{
public:
    Product (std::string const & name, double price)
            : _name(name)
            , _price(price)
    {}

    Product (std::string const & name, general::price const & price)
            : _name(name)
            , _price(price)
    {}
    
    std::string const & name () const
    {
        return _name;
    }
    
    general::price const & price () const
    {
        return _price;
    }
    
    virtual bool is_weighable () const = 0;
    virtual bool is_enumerable () const = 0;
    virtual general::cost cost () const = 0;

private:    
    std::string     _name;
    general::price  _price;
    std::vector<std::weak_ptr<Category>> _parents;
};

class Fruit : public Product, public Weighable
{
    general::color _color;
    
protected:
    Fruit (std::string const & name
            , general::price const & price
            , general::weight const & weight)
        : Product(name, price)
        , Weighable(weight)
    {
        setColor(general::color::UNSPEC);
    }

public:
    void setColor (general::color const & color)
    {
        _color = color;
    }
    
    general::color const & color () const
    {
        return _color;
    }
    
    virtual bool is_weighable () const override { return true; }
    virtual bool is_enumerable () const override { return false; }
    
    virtual general::cost cost () const override
    {
        return calculate(price(), weight());
    }
};

class Apple : public Fruit
{
public:
    Apple (general::price const & price, general::weight const & weight) 
        : Fruit("apple", price, weight)
        
    {
        setColor(general::color::RED);
    }
};

class Orange : public Fruit
{
public:
    Orange (general::price const & price, general::weight const & weight) 
        : Fruit("orange", price, weight)
    {
        setColor(general::color::ORANGE);
    }
};

class Kiwi : public Fruit
{
public:
    Kiwi (general::price const & price, general::weight const & weight)
        : Fruit("kiwi", price, weight)
    {
        setColor(general::color::GREEN);
    }
};

class Juicer : public Product, public Enumerable
{
public:
    Juicer (general::price const & price
            , general::quantity const & quantity)
        : Product("juicer", price)
        , Enumerable(quantity)
    {}
        
    virtual bool is_weighable () const override { return false; }
    virtual bool is_enumerable () const override { return true; }

    virtual general::cost cost () const override
    {
        return calculate(price(), quantity());
    }

    // TODO Implement
    // Должно быть, по крайней мере, 2 ингридиента
    template <typename T1, typename T2, typename ...Ts>
    void makeCocktail (T1 const & , T2 const &, Ts const & ... other);
};

class Store
{
public:
    typedef std::shared_ptr<Product> product_shared_type;
    typedef std::map<std::string, product_shared_type> assortment_type;
    
public:
    Store () {}
    
    void addProduct (std::shared_ptr<Product> pproduct)
    {
        _assortment.insert(assortment_type::value_type(pproduct->name(), pproduct));
    }
    
    std::shared_ptr<Product> getProductByName (std::string const & name) const
    {
        auto it = _assortment.find(name);
        return it != _assortment.cend()
                ? it->second
                : nullptr;
    }
    
    general::cost totalCost () const
    {
        return std::accumulate(_assortment.begin(), _assortment.end()
            , general::cost(0.0f)
            , [](general::cost cost, assortment_type::value_type const & x) {
                return cost + x.second->cost();
            });
    }
    
private:
    assortment_type _assortment;
};

int main ()
{
    CategoryTree categoryTree;
    auto foodStuffs   = categoryTree.make("продукты питания");
    auto dairyProduce = categoryTree.make("молочные продукты", foodStuffs);
    auto fruits       = categoryTree.make("фрукты", foodStuffs);
    auto grocery      = categoryTree.make("бакалея", foodStuffs);
    
    std::cout << to_string(dairyProduce) << std::endl;
    std::cout << to_string(fruits) << std::endl;
    std::cout << to_string(grocery) << std::endl;
        
    // Добавим продукты в магазин
    Store store;
    store.addProduct(std::make_shared<Apple>(40.0f, 100.0f));
    store.addProduct(std::make_shared<Orange>(50.0f, 100.0f));
    store.addProduct(std::make_shared<Kiwi>(60.0f, 100.0f));
    store.addProduct(std::make_shared<Juicer>(1000.0f, 2));
    
    // Проверим наличие продуктов
    assert(store.getProductByName("apple"));
    assert(store.getProductByName("orange"));
    assert(store.getProductByName("kiwi"));
    assert(store.getProductByName("juicer"));
    
    std::string cucumber("cucumber");
    
    if (!store.getProductByName(cucumber)) {
        std::cout << "Product not found: " << cucumber << std::endl;
    }
    
    assert(store.getProductByName("apple")->is_weighable());
    assert(store.getProductByName("orange")->is_weighable());
    assert(store.getProductByName("kiwi")->is_weighable());
    assert(store.getProductByName("juicer")->is_enumerable());
    
    auto fruit = store.getProductByName("apple");
    
    // Проверим, что это фрукт (косвенно, т.к. из продуктов только фрукты являются взвешиваемыми ;) )
    if (fruit->is_weighable()) {
        std::cout << "Fruit color: " << to_string(std::static_pointer_cast<Fruit>(fruit)->color()) << std::endl;
    }
    
    // Вычислим полную стоимость всех продуктов
    assert(store.totalCost() == general::cost(40.0f * 100.0f + 50.0f * 100.0f + 60.0f * 100.0f + 1000.0f * 2));
    
    std::cout << "Total cost: " << store.totalCost() << std::endl;
    
    return 0;
}
