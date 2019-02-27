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
    OperationWorker (DoItFunc doIt
            , RequestQueue & requestQueueRef
            , ResultQueue & resultQueueRef)
        : AbstractWorker(requestQueueRef, resultQueueRef)
        , _doIt(doIt)
        , _timeout{0}
    {}

    Q_SLOT   void calculate ();
    Q_SLOT   void setTimeout (int t) { _timeout = t; }
    Q_SIGNAL void resultReady ();

private:
    DoItFunc _doIt;
    int      _timeout;
};

class ControlWorker : public AbstractWorker
{
    Q_OBJECT

public:
    ControlWorker (RequestQueue & requestQueueRef
            , ResultQueue & resultQueueRef)
        : AbstractWorker(requestQueueRef, resultQueueRef)
    {}

    Q_SLOT void calculateRequested (Operation const &);
    Q_SLOT void processResult ();
    Q_SIGNAL void requestReady ();
    Q_SIGNAL void resultReady (Result const &);
};
