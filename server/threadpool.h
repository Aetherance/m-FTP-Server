#ifndef THREADPOOL
#define THREADPOOL

#include<pthread.h>
#include<iostream>
#include<vector>
#include<queue>
#include<functional>
#include<unistd.h>
using namespace std;

class threadpool
{
public:
    threadpool(int);
    ~threadpool();
    static void * worker(void *);
    void submit(function<void()>);
    void stop();
private:
    bool Stop;
    vector<pthread_t>threads;
    queue<function<void()>>TaskQueue;

    pthread_mutex_t mutex;
    pthread_cond_t task_cond;

    pthread_mutex_t mutex_wait_all;
    pthread_cond_t cond_wait_all;
    int threadsActive;
};

#endif