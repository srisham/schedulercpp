#include <chrono>
#include <iostream>
#include <ctime>
#include <time.h>
#include <iomanip>

#include "jobScheduler.h"


JobScheduler::JobScheduler() :
    m_shutdown(false) {

    m_jobTd = std::thread(&JobScheduler::handleJobThread, this);
    n_timerTd = std::thread(&JobScheduler::TimerThread, this);
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

}

void JobScheduler::add(Job k)
{
    
    // auto now = std::chrono::system_clock::now();
    // long delay_ms = delayToRun * 1000;

    // std::chrono::milliseconds duration (delay_ms);
    // k.tp = now + duration;

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
        std::lock_guard<std::mutex> lck{m_timerMtx};

        m_priorityQ.push({k});
        m_timerCV.notify_one();
    }
}

void JobScheduler::handleJobThread()
{
    while (!m_shutdown)
    {
        std::unique_lock<std::mutex> lck{m_jobMtx};

        m_jobCV.wait(lck, [this] { return m_readyToRunList.size() != 0 ||
            m_shutdown; } );

        // std::cout << "handleJobThread m_readyToRunList.size " << m_readyToRunList.size() << std::endl;
        if (m_shutdown)
            break;

        auto Job = m_readyToRunList.back();
        m_readyToRunList.pop_back();

        lck.unlock();
        Job.funcPtr(Job.st);
    }
}

void JobScheduler::TimerThread()
{
    while (!m_shutdown)
    {
        std::unique_lock<std::mutex> lck{m_timerMtx};

        if (m_shutdown) {
            break;
        }

        auto duration = std::chrono::nanoseconds(1000000000);
        // std::cout << "TimerThread m_priorityQ size " << m_priorityQ.size() << std::endl;
        if (m_priorityQ.size() != 0) {

            auto now = std::chrono::system_clock::now();

            auto head = m_priorityQ.top();
            duration = head.tp - now;
            time_t tm = std::chrono::system_clock::to_time_t(head.tp);

            if (now >= head.tp) {

                std::unique_lock<std::mutex> ulck{m_jobMtx};

                m_readyToRunList.push_back(head);
                std::cout << "Job Scheduled @ " << std::put_time(std::localtime(&tm), "%F %T") << std::endl;

                m_jobCV.notify_one();
                ulck.unlock();

                m_priorityQ.pop();
            }
        }
        std::cout << "timer thread loop " << std::endl;
        m_timerCV.wait_for(lck, duration);
    }
}

