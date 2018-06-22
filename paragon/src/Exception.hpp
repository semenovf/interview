#pragma once
#include <stdexcept>
#include <QString>

#define ASSERT(expr) if (!(expr)) throw Exception(#expr);
#define ASSERT_X(expr,msg) if (!(expr)) throw Exception(msg);

class Exception : public std::runtime_error
{
    using base_class = std::runtime_error;
public:
    explicit Exception (char const * s) : base_class(s) {}
    explicit Exception (std::string const & s) : base_class(s) {}
    explicit Exception (QString const & s) : base_class(s.toUtf8().constData()) {}
};
