#pragma once
#include <QObject>
#include "Operation.hpp"
#include "Result.hpp"
#include "Queue.hpp"

extern double DoIt (int TypeWork
        , double OperandA
        , double OperandB
        , int & ErrorCode);

using RequestQueue = Queue<Operation>;
using ResultQueue  = Queue<Result>;
using DoItFunc = double (*) (int TypeWork
        , double OperandA
        , double OperandB
        , int & ErrorCode);

class AbstractWorker : public QObject
{
    Q_OBJECT

public:
    Q_SIGNAL void requestQueueSizeChanged (int sz);
    Q_SIGNAL void resultQueueSizeChanged (int sz);

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
    Q_OBJECT

public:
    OperationWorker (DoItFunc func
            , RequestQueue & requestQueueRef
            , ResultQueue & resultQueueRef)
        : AbstractWorker(requestQueueRef, resultQueueRef)
        , _func(func)
    {}

protected:
    Q_SLOT void calculate ();
    Q_SIGNAL void resultReady (Result const &);

private:
    DoItFunc _func;
};

class ControlWorker : public AbstractWorker
{
    Q_OBJECT

public:
    ControlWorker (RequestQueue & requestQueueRef
            , ResultQueue & resultQueueRef)
        : AbstractWorker(requestQueueRef, resultQueueRef)
    {}

    Q_SLOT void requested (Operation const &);
    Q_SLOT void onResultReady () {}
    Q_SIGNAL void requestReady ();
};
