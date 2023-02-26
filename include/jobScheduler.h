#pragma once
#include <iostream>
#include <chrono>
#include <queue>
#include <unistd.h>
#include <vector>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <memory>
#include <functional>


struct Payload {
    std::string deviceType;
    std::string name;
    int sceneId;
};

typedef std::function<std::string(std::shared_ptr<Payload>)> ExecuteJob;
using timepoint = std::chrono::time_point<std::chrono::steady_clock>;

struct Job {
    timepoint tp;
    std::shared_ptr<Payload> st;
    ExecuteJob funcPtr;
};


class JobScheduler {

public:
    JobScheduler();
    ~JobScheduler();

    void add(Job k, double delayToRun);

private:

    struct TScomp {
        bool operator()(const Job &a, const Job &b) const
        {
            return a.tp > b.tp;
        }
    };

    std::vector<Job> m_readyToRunList;
    std::priority_queue<Job, std::vector<Job>, TScomp> m_priorityQ;

    std::thread n_timerTd, m_jobTd;
    std::mutex m_timerMtx, m_jobMtx;
    std::condition_variable m_timerCV, m_jobCV;

    bool m_shutdown;

    void worker_thread();
    void timer_thread();
};