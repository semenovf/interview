#include "ErrorCode.hpp"
#include "Calculator.hpp"

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

Calculator::Calculator ()
{
    OperationWorker * operationWorker = new OperationWorker(
              DoIt
            , _requestQueueRef
            , _resultQueueRef);

    ControllerWorker * controllerWorker = new ControllerWorker(
              _requestQueueRef
            , _resultQueueRef);
    
    operationWorker->moveToThread(& _operationThread);
    controllerWorker->moveToThread(& _controllerThread);
    
    QObject::connect(& _operationThread, & QThread::finished
            , operationWorker, & QObject::deleteLater);
    QObject::connect(& _controllerThread, & QThread::finished
            , controllerWorker, & QObject::deleteLater);
//     connect(this, &Controller::operate, worker, &Worker::doWork);
//     connect(worker, &Worker::resultReady, this, &Controller::handleResults);
    _operationThread.start();
}

Calculator::~Calculator()
{
    _operationThread.quit();
    _controllerThread.quit();
    _operationThread.wait();
    _controllerThread.wait();        
}
