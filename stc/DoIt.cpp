#include "Operation.hpp"
#include "ErrorCode.hpp"

double DoIt (int typeWork
        , double operandA
        , double operandB
        , int & errorCode)
{
    double result = 0;
    switch (typeWork) {
        case static_cast<int>(Operator::PLUS):
            result = operandA + operandB;
            break;
        case static_cast<int>(Operator::MINUS):
            result = operandA - operandB;
            break;
        case static_cast<int>(Operator::MULTIPLY):
            result = operandA * operandB;
            break;
        case static_cast<int>(Operator::DIVIDE):
            result = operandA / operandB;
            break;
        default:
            errorCode = BAD_OPERATOR_ERROR;
            break;
    }

    return result;
}
