#pragma once

enum class Operator {
      UNKNOWN
    , PLUS
    , MINUS
    , MULTIPLY
    , DIVIDE
};

class Operation
{
public:
    Operation (Operator op, double operandA, double operandB)
        : _op(op)
        , _op1(operandA)
        , _op2(operandB)
    {}

    Operation (Operation const & ) = default;
    Operation (Operation && ) = default;
    Operation & operator = (Operation const & ) = default;
    Operation & operator = (Operation && ) = default;
    ~Operation () = default;
    
private:
    Operator _op;
    double   _op1;
    double   _op2;
};
