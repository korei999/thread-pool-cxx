#pragma once

#include <condition_variable>
#include <functional>
#include <queue>

class ThreadPool
{
public:
    void
    start()
    {
        const size_t nThreads = std::thread::hardware_concurrency();
        for (size_t i = 0; i < nThreads; i++)
            aThreads.emplace_back(std::thread(&ThreadPool::loop, this));
    }

    void
    qJob(const std::function<void()>& job)
    {
        {
            std::unique_lock lock(mtxQ);
            qTasks.push(job);
        }
        cndMtx.notify_one();
    }

    void
    stop()
    {
        bDone = true;
        cndMtx.notify_all();
        for (std::thread& activeThread : aThreads)
            activeThread.join();
    }

    bool
    busy()
    {
        bool bPoolBusy;
        {
            std::unique_lock lock(mtxQ);
            bPoolBusy = !qTasks.empty();
        }
        return bPoolBusy;
    }

    void
    wait()
    {
        if (busy())
        {
            std::unique_lock lock(mtxWait);
            cndWait.wait(lock);
        }
    }

private:
    void
    loop()
    {
        while (!bDone)
        {
            std::function<void()> job;
            {
                std::unique_lock lock(mtxQ);

                cndMtx.wait(lock, [this] { return !qTasks.empty() || bDone; });

                if (bDone)
                    break;

                job = qTasks.front();
                qTasks.pop();

                if (qTasks.empty()) cndWait.notify_one();
            }
            job();
        }
    }

    std::atomic<bool> bDone = false; /* tells threads to stop looking for jobs */
    std::mutex mtxQ; /* prevents data races to the job q */
    std::condition_variable cndMtx; /* allows threads to wait on new jobs or termination */
    std::mutex mtxWait;
    std::condition_variable cndWait;
    std::vector<std::thread> aThreads;
    std::queue<std::function<void()>> qTasks;
};
