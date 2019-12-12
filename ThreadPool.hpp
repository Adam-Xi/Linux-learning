#pragma once
#include <stdio.h>
#include "BlockingQueue.hpp"

class Task
{
public:
    virtual ~Task()
    {}
    //虚函数的应用场景：
    virtual void Run()
    {
        printf("base Run\n");
    }
};


//线程池对象启动的时候会创建一组线程
//每个线程都需要完成一定的任务（执行一定的代码逻辑, 这个逻辑是由调用者来决定的）
//任务是一段代码，可以用函数表示
//
//
//
//
//提前把线程创建好，避免频繁创建销毁线程的开销
//线程不必一次创建太多，复用一个线程来完成多个任务
class ThreadPool
{
public:
    //n表示创建线程的数量
    ThreadPool(int n)
        : queue_(100)
        , worker_count_(n)
    {
        //创建出若干个线程
        for(int i = 0; i < n; i++)
        {
            pthread_t tid;
            pthread_create(&tid, NULL, ThreadEntry, this);
            workers_.push_back(tid);
        }
    }

    ~ThreadPool()
    {
        //先让线程退出，然后再回收
        for(size_t i = 0; i < workers_.size(); i++)
        {
            pthread_cancel(workers_[i]);
        }
        for(size_t i = 0; i < workers_.size(); i++)
        {
            pthread_join(workers_[i], NULL);
        }
    }

    //使用线程池的时候，需要调用者加入一些任务，让线程去执行
    void AddTask(Task *task)
    {
        queue_.Push(task);
    }
    
private:
    BlockingQueue<Task*> queue_;
    int worker_count_;
    std::vector<pthread_t> workers_;

    static void *ThreadEntry(void *arg)
    {
        ThreadPool *pool = (ThreadPool*)arg;
        while(true)
        {
            //循环中尝试从阻塞队列中获取任务并且执行
            Task *task = NULL;
            pool->queue_.Pop(&task);
            //task表面上看是一个Task*类型，实际上指向的是MyTask*
            //执行的是子类，用户自定制的逻辑
            task->Run();
            delete task;
        }
    }
};

