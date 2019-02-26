#include "Operation.hpp"

QString toString (Operator op)
{
    switch (op) {
        case Operator::PLUS: return QString{"+"};
        case Operator::MINUS: return QString{"-"};
        case Operator::MULTIPLY: return QString{"*"};
        case Operator::DIVIDE: return QString{"/"};
    }
    return QString{"?"};
}
