#pragma once
#include <QMutex>
#include <QMutexLocker>
#include <QQueue>
#include "Operation.hpp"

template <typename T>
class Queue
{
public:
    void dequeue (int & sz)
    {
        QMutexLocker locker(& _m);
        _q.dequeue();
        sz = _q.size();
    }

    void enqueue (T const & op, int & sz)
    {
        QMutexLocker locker(& _m);
        _q.enqueue(op);
        sz = _q.size();
    }

    T head (int & sz) const
    {
        QMutexLocker locker(& _m);
        T r = _q.head();
        sz = _q.size();
        return r;
    }

private:
    QMutex    _m;
    QQueue<T> _q;
};
