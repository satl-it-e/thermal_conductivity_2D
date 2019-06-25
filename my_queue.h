#ifndef THERMAL_CONDUCTIVITY_2D_MY_QUEUE_H
#define THERMAL_CONDUCTIVITY_2D_MY_QUEUE_H


#include <iostream>
#include <mutex>
#include <queue>


template<typename T>
class MyQueue{

private:
    std::mutex mtx;
    std::condition_variable cv;
    std::queue<T> *q = new std::queue<T>;
    bool notified = false;
    bool finished = false;

public:
    void finish(){
        std::lock_guard<std::mutex> lock(mtx);
        finished = true;
        notified = true;
        cv.notify_all();
    }

    void push(T element){
        std::unique_lock<std::mutex> lock(mtx);
        q->push(element);
        notified = true;
        cv.notify_one();
    }

    std::vector<T> pop(){
        std::vector<T> some;
        std::unique_lock<std::mutex> lock(mtx);
        while (!finished || (q->size() >= 1)) {
            while(!notified && q->empty()){
                cv.wait(lock);
            }
            while(q->size() >= 1) {
                some.emplace_back(q->front());
                q->pop();
                return some;
            }
            notified = false;
        }
        return some;
    }
};


#endif //THERMAL_CONDUCTIVITY_2D_MY_QUEUE_H
