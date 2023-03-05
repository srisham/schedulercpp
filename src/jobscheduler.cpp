#include <chrono>
#include <iostream>
#include <ctime>
#include <time.h>
#include <iomanip>

#include "jobscheduler.h"


JobScheduler::JobScheduler() :
    m_shutdown(false) {

    m_jobTd = std::thread(&JobScheduler::executeJobThread, this);
    m_timerTd = std::thread(&JobScheduler::scheduleJobThread, this);
}

JobScheduler::~JobScheduler() {

    m_shutdown = true;
    
    {
        std::lock_guard<std::mutex> lck{m_timerMtx};
        m_timerCV.notify_all();
    }
    if (m_timerTd.joinable()) {
        m_timerTd.join();
    }

    {
        std::lock_guard<std::mutex> lck{m_jobMtx};
        m_jobCV.notify_all();
    }
    if (m_jobTd.joinable()) {
        m_jobTd.join();
    }
}

void JobScheduler::add(Job jb)
{
    std::lock_guard<std::mutex> lck{m_timerMtx};

    std::cout << "New Job pushed to Queue" << std::endl;
    m_priorityQ.push({jb});
    m_timerCV.notify_one();
}

void JobScheduler::executeJobThread()
{
    while (!m_shutdown) {
        
        std::unique_lock<std::mutex> lck{m_jobMtx};

        m_jobCV.wait(lck, [this] { return m_readyToRunList.size() != 0 ||
            m_shutdown; } );

        if (m_shutdown) {
            break;
        }

        auto Job = m_readyToRunList.back();
        m_readyToRunList.pop_back();

        lck.unlock();
        std::cout << "Job executed " << std::endl;
        Job.funcPtr(Job.payload);
    }
}

void JobScheduler::scheduleJobThread()
{
    while (!m_shutdown)
    {
        std::unique_lock<std::mutex> lck{m_timerMtx};
         m_timerCV.wait(lck, [this] { 
                return (m_priorityQ.size() > 0 || m_shutdown);
        });

        if (m_shutdown) {
            break;
        }

        if (m_priorityQ.size() != 0) {

            auto now = std::chrono::system_clock::now();

            auto head = m_priorityQ.top();
            auto duration = head.tp - now;
            time_t tm = std::chrono::system_clock::to_time_t(head.tp);

            if (now >= head.tp) {

                std::unique_lock<std::mutex> ulck{m_jobMtx};

                m_readyToRunList.push_back(head);

                m_jobCV.notify_one();
                ulck.unlock();

                m_priorityQ.pop();
            }
            else {
                std::cout << "Job scheduled @ " << std::put_time(std::localtime(&tm), "%F %T") << std::endl;
                m_timerCV.wait_for(lck, duration);
            }
        }
    }
}

