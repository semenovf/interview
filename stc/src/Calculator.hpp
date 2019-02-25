#pragma once
#include <QThread>
#include "Worker.hpp"

class Calculator
{
public:
    Calculator ();
    ~Calculator ();
    
private:
    QThread _operationThread;
    QThread _controllerThread;
    RequestQueue _requestQueueRef;
    ResultQueue  _resultQueueRef;
};
