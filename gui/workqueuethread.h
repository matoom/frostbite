#ifndef WORKQUEUETHREAD_H
#define WORKQUEUETHREAD_H

#include <QThread>
#include "concurrentqueue.h"

template <typename T>
class WorkQueueThread : public QThread {
public:
    WorkQueueThread(QObject *parent = nullptr) : QThread(parent) {}
    
    ~WorkQueueThread() {
        stop();
        if(!this->wait(1000)) {
            qWarning("Thread deadlock detected, terminating thread.");
            this->terminate();
            this->wait();
        }        
    }

    void stop() {
        this->requestInterruption();
        dataQueue.stop();
    }
    
    void addData(const T& data) {
        dataQueue.push(data);        
    }
    
protected:
    virtual void run() {
        while (!this->isInterruptionRequested()) {
            T localData;
            if (dataQueue.waitAndPop(localData)) {
                onProcess(localData);
            } else {
                break;
            }
        }
    }
    
    virtual void onProcess(const T& data) = 0;
private:
    ConcurrentQueue<T> dataQueue;


};

#endif // WORKQUEUETHREAD_H
