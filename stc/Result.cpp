#include "ErrorCode.hpp"
#include "Result.hpp"

QString Result::toErrorString (int errorCode)
{
    switch (errorCode) {
        case NO_ERROR: return QString{"No error"};
        case DIVIDE_BY_ZERO: return QString{"Divide by zero"};
        case BAD_OPERATOR_ERROR: return QString{"Bad operator"};
    }

    return QString{"Unknown error code: " + QString::number(errorCode)};
}

