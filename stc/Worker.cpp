#include "Worker.hpp"

#include <QDebug>

void OperationWorker::calculate ()
{
    int sz = 0;

    do {
        Operation op = _requestQueueRef.head(sz);
        if (sz) {
            _requestQueueRef.dequeue(sz);
            emit requestQueueSizeChanged(sz);

            //emit requestQueueSizeChanged(sz);


        }
    } while (sz);
}

void ControlWorker::requested (Operation const & p)
{
    int sz = 0;
    _requestQueueRef.enqueue(p, sz);
    emit requestQueueSizeChanged(sz);
}
