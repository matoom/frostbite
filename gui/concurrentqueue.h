#ifndef CONCURRENT_QUEUE_H
#define CONCURRENT_QUEUE_H

#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>

template <typename T>
class ConcurrentQueue {
public:
    ConcurrentQueue() = default;
    ~ConcurrentQueue() = default;

    bool empty() const { 
        QMutexLocker lock(&mutex);
        return queue.isEmpty();
    }
    
    void push(T const& data) {
        mutex.lock();
        const auto wasEmpty = queue.isEmpty();
        queue.enqueue(data);
        mutex.unlock();
        // signal if the data was added to the empty queue
        if (wasEmpty) {
            queueCondition.wakeOne();
        }
    }

    bool waitAndPop(T& data) {
        QMutexLocker lock(&mutex);
        while(queue.isEmpty()) {
            queueCondition.wait(&mutex);
            if (shouldStop) return false;
        }
        data = queue.dequeue();
        return true;
    }

    void stop() {
        QMutexLocker lock(&mutex);
        shouldStop = true;
        queueCondition.wakeOne();
    }
    
private:
    QWaitCondition queueCondition;
    mutable QMutex mutex;
    QQueue<T> queue;
    bool shouldStop = false;
};

#endif // CONCURRENT_QUEUE_H
