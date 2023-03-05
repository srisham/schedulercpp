#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <time.h>
#include <chrono>
#include <any>

#include "jobscheduler.h"

using namespace std::chrono;

struct Payload {
    std::string deviceType;
    std::string name;
    int sceneId;
};

void printCurrentTime() {

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::cout << std::put_time(std::localtime(&now_c), "%F %T") << std::endl;

    //   << "Epoch Time "
    //   << duration_cast<std::chrono::seconds>(now.time_since_epoch()).count()
    //   << std::endl;

}

time_t getFutureTime(uint32_t hour, uint32_t minutes) {
    
    struct tm * timeinfo;

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t nowTm = std::chrono::system_clock::to_time_t(now);

    timeinfo = std::localtime(&nowTm);
    timeinfo->tm_hour = hour;
    timeinfo->tm_min = minutes;

    time_t futureTm = mktime(timeinfo);

    return futureTm;
}

void handleRequest(std::any arg) {

    Payload payload;
    try {
        payload = std::any_cast<Payload>(arg);
    }
    catch(const std::bad_any_cast& e) {
        std::cout << "Invalid arguments type: " << e.what() << std::endl;
        return;
    }

    std::cout << "handleRequest @ ";  printCurrentTime();
    std:: cout << "Received Arguments are " 
        << payload.name << " "
        << payload.deviceType << " "
        << payload.sceneId << " "
        << std::endl;
}

int main() {    

    std::cout << "Job Scheduler example " << std::endl;

    Payload p1 = {"Wiz", "light", 12};
    Payload p2 = {"Wemo", "plug", 14};
    Payload p3 = {"Wyze", "light", 32};

    Job jb1, jb2, jb3;
    jb1.payload = p1;
    jb1.tp = std::chrono::system_clock::from_time_t(getFutureTime(10, 8)); // 10:08 (HH:MM)
    jb1.funcPtr = handleRequest;

    jb2.payload = p2;
    jb2.tp = std::chrono::system_clock::from_time_t(getFutureTime(23, 9)); // 23:09 (HH:MM)
    jb2.funcPtr = handleRequest;

    JobScheduler sched;
    sched.add(jb1);
    sched.add(jb2); 

    std::this_thread::sleep_for(std::chrono::minutes(1));
    jb3.payload = p3;
    jb3.tp = std::chrono::system_clock::from_time_t(getFutureTime(10, 8)); // 10:08 (HH:MM)
    jb3.funcPtr = handleRequest;
    sched.add(jb3);

    while (1) {}
}
