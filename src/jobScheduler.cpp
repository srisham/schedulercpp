#include <chrono>
#include <iostream>

#include "jobScheduler.h"


JobScheduler::JobScheduler() :
    m_shutdown(false) {

    m_jobTd = std::thread(&JobScheduler::worker_thread, this);
    n_timerTd = std::thread(&JobScheduler::timer_thread, this);
}

JobScheduler::~JobScheduler() {


    m_shutdown = true;

    {
        std::lock_guard<std::mutex> lck{m_timerMtx};
        m_timerCV.notify_one();
    }
    n_timerTd.join();

    {
        std::lock_guard<std::mutex> lck{m_jobMtx};
        m_jobCV.notify_all();
    }
    m_jobTd.join();

    // for (auto &t : ThreadPool)
    //     t.join();
}

void JobScheduler::add(Job k, double delayToRun)
{

    auto now = std::chrono::steady_clock::now();
    long delay_ms = delayToRun * 1000;

    std::chrono::milliseconds duration (delay_ms);
    k.tp = now + duration;

    // if (now >= tp)
    // {
    //     /*
    //      * This is a short-cut
    //      * When time is due, the task is directly dispatched to the workers
    //      */
    //     std::lock_guard<std::mutex> lck{m_jobMtx};
    //     m_readyToRunList.push_back(&task);
    //     m_jobCV.notify_one();

    // } else
    {
        // std::cout << "m_priorityQ pushed " << std::endl;
        std::lock_guard<std::mutex> lck{m_timerMtx};

        m_priorityQ.push({k});

        m_timerCV.notify_one();
    }
}

void JobScheduler::worker_thread()
{
    std::cout << "worker_thread + " << std::endl;
    for (;;)
    {
        std::unique_lock<std::mutex> lck{m_jobMtx};

        m_jobCV.wait(lck, [this] { return m_readyToRunList.size() != 0 ||
            m_shutdown; } );

        // std::cout << "worker_thread m_readyToRunList.size " << m_readyToRunList.size() << std::endl;
        if (m_shutdown)
            break;

        auto Job = m_readyToRunList.back();
        m_readyToRunList.pop_back();

        lck.unlock();

        Job.funcPtr(Job.st);
        // p->run();

        // delete p; // delete Task
    }
    std::cout << "worker_thread - " << std::endl;
}

void JobScheduler::timer_thread()
{
    std::cout << "timer_thread + " << std::endl;
    for (;;)
    {
        std::unique_lock<std::mutex> lck{m_timerMtx};

        if (m_shutdown)
            break;

        auto duration = std::chrono::nanoseconds(1000000000);

        // std::cout << "timer_thread m_priorityQ size " << m_priorityQ.size() << std::endl;
        if (m_priorityQ.size() != 0)
        {
            auto now = std::chrono::steady_clock::now();

            auto head = m_priorityQ.top();
            // auto devState = head.st;

            duration = head.tp - now;
            if (now >= head.tp)
            {
                /*
                 * A Task is due, pass to worker threads
                 */
                std::unique_lock<std::mutex> ulck{m_jobMtx};
                m_readyToRunList.push_back(head);
                // std::cout << "timer_thread m_readyToRunList element pushed" << std::endl;
                m_jobCV.notify_one();
                ulck.unlock();

                m_priorityQ.pop();
            }
        }

        m_timerCV.wait_for(lck, duration);
    }
    std::cout << "timer_thread - " << std::endl;
}

