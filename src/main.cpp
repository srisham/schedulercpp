#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <time.h>
#include <chrono>

#include "jobscheduler.h"

using namespace std;
using namespace std::chrono;

struct Payload {
    std::string deviceType;
    std::string name;
    int sceneId;
};

void printCurrentTime() {

    // time_t utc_now = time( nullptr );
    // cout << utc_now << endl; // Time since Epoch

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::cout << std::put_time(std::localtime(&now_c), "%F %T") << "\t"
              << std::endl;
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

    if (futureTm < nowTm) {
        std::cout << "Time is expired " << std::endl;
    }
    /*
    else {
        std::cout << "Future time is " << std::put_time(std::localtime(&futureTm), "%F %T")  << std::endl;
    }
    */

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

    std::cout << "handleRequest @ ";
    printCurrentTime();
    std:: cout << "Received Arguments are " 
        << payload.name << " "
        << payload.deviceType << " "
        << payload.sceneId << " "
        << std::endl;
}

int main() {    

    std::cout << "Job Scheduler " << std::endl;

    Payload p1 = {"Wiz", "light", 12};
    Payload p2 = {"Wemo", "plug", 14};
    Payload p3 = {"Wyze", "light", 32};

    Job j1, j2, j3;
    j1.payload = p1;
    j1.tp = std::chrono::system_clock::from_time_t(getFutureTime(10, 8));
    j1.funcPtr = handleRequest;

    j2.payload = p2;
    j2.tp = std::chrono::system_clock::from_time_t(getFutureTime(10, 9));
    j2.funcPtr = handleRequest;

    JobScheduler sched;
    sched.add(j1);
    sched.add(j2); 

    std::this_thread::sleep_for(std::chrono::minutes(1));
    j3.payload = p3;
    j3.tp = std::chrono::system_clock::from_time_t(getFutureTime(10, 8));
    j3.funcPtr = handleRequest;
    sched.add(j3);

    std::this_thread::sleep_for(std::chrono::minutes(4));
}
