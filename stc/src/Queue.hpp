#pragma once
#include <QMutex>
#include <QMutexLocker>
#include <QQueue>
#include "Operation.hpp"

template <typename T>
class Queue
{
public:
    void dequeue () 
    {
        QMutexLocker locker(& _m);
        _q.dequeue();
    }
    
    void enqueue (T const & op)
    {
         QMutexLocker locker(& _m);
         _q.enqueue(op);
    }
    
    T head () const
    {
        QMutexLocker locker(& _m);
        T r = _q.head();
        return r;
    }

private:
    QMutex    _m;
    QQueue<T> _q;
};
