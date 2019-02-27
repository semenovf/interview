#pragma once
#include <QString>
#include "Result.hpp"

using DoItFunc = double (*) (int TypeWork
        , double OperandA
        , double OperandB
        , int & ErrorCode);

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
    Operation ()
        : _op{Operator::UNKNOWN}
        , _op1{0}
        , _op2{0}
    {}

    Operation (Operator op, double operandA, double operandB)
        : _op{op}
        , _op1{operandA}
        , _op2{operandB}
    {}

    Operation (Operation const & ) = default;
    Operation (Operation && ) = default;
    Operation & operator = (Operation const & ) = default;
    Operation & operator = (Operation && ) = default;
    ~Operation () = default;

    Operator getOperator () const { return _op; }
    double getOperandA () const   { return _op1; }
    double getOperandB () const   { return _op2; }

private:
    Operator _op;
    double   _op1;
    double   _op2;
};

QString toString (Operator op);
