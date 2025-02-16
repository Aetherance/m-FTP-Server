#include"threadpool.h"
threadpool::threadpool(int Size) {
    pthread_mutex_init(&mutex,nullptr);
    pthread_cond_init(&task_cond,nullptr);
    pthread_mutex_init(&mutex_wait_all,nullptr);
    pthread_cond_init(&cond_wait_all,nullptr);

    pthread_t temp_ptd;
    for(int i = 0;i<Size;i++) {
        pthread_create(&temp_ptd,nullptr,worker,this);
        threads.push_back(temp_ptd);
    }
    printf("Created %d threads for service\n",Size);
    Stop = false;
    threadsActive = 0;
}

void * threadpool::worker(void * this_ptr) {
    threadpool * pthis = static_cast<threadpool*>(this_ptr);
    while(true) {
        pthread_mutex_lock(&pthis->mutex);
        while(pthis->TaskQueue.empty()&&!pthis->Stop) {
            pthread_cond_wait(&pthis->task_cond,&pthis->mutex);
        }

        if(pthis->Stop&&pthis->TaskQueue.empty()) {
            printf("Thread %ld exited\n",pthread_self());
            pthread_mutex_unlock(&pthis->mutex);
            return nullptr;
        }
        function<void()>task = pthis->TaskQueue.front();
        pthis->TaskQueue.pop();
        pthis->threadsActive ++;
        pthread_mutex_unlock(&pthis->mutex);
        task();
        pthread_mutex_lock(&pthis->mutex);
        pthis->threadsActive --;

        if(pthis->TaskQueue.empty()&&!pthis->threadsActive)
            pthread_cond_broadcast(&pthis->cond_wait_all);
        
        pthread_mutex_unlock(&pthis->mutex);
    }
    return nullptr;
}

void threadpool::submit(function<void()>task) {
    TaskQueue.push(task);
    pthread_cond_signal(&task_cond);
}

void threadpool::stop() {
    pthread_mutex_lock(&mutex_wait_all);
    while(TaskQueue.size() || threadsActive) {
        pthread_cond_wait(&cond_wait_all,&mutex_wait_all);
    }
    pthread_mutex_unlock(&mutex_wait_all);
    Stop = true;
    pthread_cond_broadcast(&task_cond);
    for(pthread_t thread : threads) {
        pthread_join(thread,nullptr);
    }
}   

threadpool::~threadpool() {
    if(!Stop) {
        stop();
    }
    pthread_mutex_destroy(&mutex_wait_all);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_wait_all);
    pthread_cond_broadcast(&cond_wait_all);
}
