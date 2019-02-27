#include "ErrorCode.hpp"

extern "C" {
double DoIt (int typeWork
        , double operandA
        , double operandB
        , int & errorCode)
{
    double result = double{0};

    switch (typeWork) {
        case 1: // PLUS
            result = operandA + operandB;
            break;
        case 2: // MINUS
            result = operandA - operandB;
            break;
        case 3: // MULTIPLY
            result = operandA * operandB;
            break;
        case 4: // DIVIDE
            if (operandB == double{0}) {
                errorCode = DIVIDE_BY_ZERO;
            } else {
                result = operandA / operandB;
            }
            break;
        default:
            errorCode = BAD_OPERATOR_ERROR;
            break;
    }

    return result;
}

} // extern "C"
