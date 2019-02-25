#pragma once
#include <QObject>
#include "Operation.hpp"
#include "Result.hpp"
#include "Queue.hpp"

using RequestQueue = Queue<Operation>;
using ResultQueue  = Queue<Result>;
using DoItFunc = double (*) (int TypeWork
        , double OperandA
        , double OperandB
        , int & ErrorCode);

// extern double DoIt (int TypeWork
//         , double OperandA
//         , double OperandB
//         , int & ErrorCode);

class AbstractWorker : public QObject
{
protected:
    AbstractWorker (RequestQueue & requestQueueRef
            , ResultQueue & resultQueueRef)
        : _requestQueueRef(requestQueueRef)
        , _resultQueueRef(resultQueueRef)
    {}
    
protected:
    RequestQueue & _requestQueueRef;
    ResultQueue &  _resultQueueRef;
};

class OperationWorker : public AbstractWorker
{
public:
    OperationWorker (DoItFunc func
            , RequestQueue & requestQueueRef
            , ResultQueue & resultQueueRef)
        : AbstractWorker(requestQueueRef, resultQueueRef)
        , _func(func)
    {}
    
private:
    DoItFunc _func;
};

class ControllerWorker : public AbstractWorker
{
public:
    ControllerWorker (RequestQueue & requestQueueRef
            , ResultQueue & resultQueueRef)
        : AbstractWorker(requestQueueRef, resultQueueRef)
    {}
};
