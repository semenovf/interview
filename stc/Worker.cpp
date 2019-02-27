#include <QThread>
#include <QTime>
#include "Worker.hpp"

#include <QDebug>

class Timer
{
public:
    Timer () : _timeout{0} {}

    void start (int timeout)
    {
        _timeout = timeout;
        _timer.start();
    }

    void wait ()
    {
        int remainSeconds = _timeout - _timer.elapsed() * 1000;

        if (remainSeconds > 0) {
            qDebug() << "Remain seconds" << remainSeconds;
            QThread::sleep(remainSeconds);
        }
    }
private:
    int   _timeout;
    QTime _timer;
};

void OperationWorker::calculate ()
{
    int sz = 0;

    do {
        Operation op = _requestQueueRef.head(sz);

        if (sz) {
            _requestQueueRef.dequeue(sz);
            emit requestQueueSizeChanged(sz);

            // Emulate long time operation
            Timer timer;
            timer.start(_timeout);

            int errorCode = 0;
            double d = _doIt(static_cast<int>(op.getOperator())
                    , op.getOperandA()
                    , op.getOperandB()
                    , errorCode);

            timer.wait();

            Result result = errorCode
                    ? Result::makeError(errorCode)
                    : Result::makeOk(d);

            int sz1 = 0;
            _resultQueueRef.enqueue(result, sz1);
            emit resultQueueSizeChanged(sz1);
            emit resultReady();
        }
    } while (sz);
}

void ControlWorker::calculateRequested (Operation const & p)
{
    int sz = 0;
    _requestQueueRef.enqueue(p, sz);
    emit requestQueueSizeChanged(sz);
    emit requestReady();
}

void ControlWorker::processResult ()
{
    int sz = 0;

    do {
        Result res = _resultQueueRef.head(sz);

        if (sz) {
            _resultQueueRef.dequeue(sz);
            emit resultQueueSizeChanged(sz);
            emit resultReady(res);
        }
    } while (sz);
}
