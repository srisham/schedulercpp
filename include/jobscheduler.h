#pragma once
#include <iostream>
#include <chrono>
#include <queue>
#include <vector>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <functional>
#include <any>

typedef std::function<void(std::any)> Executer;
using timepoint = std::chrono::time_point<std::chrono::system_clock>;

struct Job {
    timepoint tp;
    std::any payload;
    Executer funcPtr;
};

class JobScheduler {

public:
    JobScheduler();
    ~JobScheduler();

    void add(Job k);

private:
    struct TScomp {
        bool operator()(const Job &a, const Job &b) const
        {
            return a.tp > b.tp;
        }
    };

    std::vector<Job> m_readyToRunList;
    std::priority_queue<Job, std::vector<Job>, TScomp> m_priorityQ;

    std::thread m_timerTd, m_jobTd;
    std::mutex m_timerMtx, m_jobMtx;
    std::condition_variable m_timerCV, m_jobCV;

    bool m_shutdown;

    void executeJobThread();
    void scheduleJobThread();
};