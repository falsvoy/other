//
// Created by Надежда on 22.12.16.
//

#ifndef STRINGCALCULATOR_THREAD_SAFE_QUEUE_H
#define STRINGCALCULATOR_THREAD_SAFE_QUEUE_H

#include <queue>
#include <utils/threads.h>

namespace sharik
{
    using std::queue;
    using android::Mutex;
    using android::AutoMutex;
    using android::Condition;

    template <typename T>
    class Queue {
    public:
        Queue();
        void push(const T &);
        void push_back(const T &);
        void pop();
        bool empty() const;
        typename queue<T>::size_type size() const;
        typename queue<T>::reference front();
        typedef typename queue<T>::value_type value_type;
    private:
        mutable Mutex mLock;
        Condition condition;
        queue<T> q;
    };

    template <typename T>
    Queue<T>::Queue()
            : mLock(), condition(), q()
    {
    }

    template <typename T>
    void Queue<T>::push(const T &value)
    {
        AutoMutex _l(mLock);

        q.push(value);
        condition.signal();
    }

    template <typename T>
    void Queue<T>::push_back(const T &value)
    {
        push(value);
    }

    template <typename T>
    void Queue<T>::pop()
    {
        AutoMutex _l(mLock);

        q.pop();
    }

    template <typename T>
    bool Queue<T>::empty() const
    {
        AutoMutex _l(mLock);

        return q.empty();
    }

    template <typename T>
    typename queue<T>::size_type Queue<T>::size() const
    {
        AutoMutex _l(mLock);

        return q.size();
    }

    template <typename T>
    typename queue<T>::reference Queue<T>::front()
    {
        AutoMutex _l(mLock);

        if (q.empty()) {
            condition.wait(mLock);
        }

        return q.front();
    }

    void *thread1(void *data)
    {
        Queue<uint8_t> *queue = static_cast<Queue<uint8_t> *>(data);

        for (int i = 0; i < 100000; ++i) {
            queue->push(rand() % 256);
        }
        printf("thread1 done\n");
        return NULL;
    }

    void *thread2(void *data)
    {
        Queue<uint8_t> *queue = static_cast<Queue<uint8_t> *>(data);

        for (int i = 0; i < 100000; ++i) {
            queue->push(rand() % 256);
        }
        printf("thread2 done\n");
        return NULL;
    }

}


#endif //STRINGCALCULATOR_THREAD_SAFE_QUEUE_H
