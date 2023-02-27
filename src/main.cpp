#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <time.h>
#include <chrono>

#include "jobScheduler.h"

using namespace std;
using namespace std::chrono;

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
    else {
        std::cout << "Future time is " << std::put_time(std::localtime(&futureTm), "%F %T")  << std::endl;
    }

    return futureTm;
}

std::string handleRequest(std::shared_ptr<Payload> arg) {

    std::cout << "Job executed @ ";
    printCurrentTime();
    std:: cout << "Received Arguments are " 
        << arg->name << " "
        << arg->deviceType << " "
        << arg->sceneId << " "
        << std::endl;

    return "Success";
}

int main() {    

    std::cout << "Job Scheduler " << std::endl;

    auto p1 = std::make_shared<Payload>(Payload{"Wiz", "light", 12});
    auto p2 = std::make_shared<Payload>(Payload{"Wemo", "plug", 14});
    auto p3 = std::make_shared<Payload>(Payload{"Wyze", "light", 32});

    Job k1, k2, k3;
    k1.st = p1;
    k1.tp = std::chrono::system_clock::from_time_t(getFutureTime(15, 30));
    k1.funcPtr = handleRequest;

    k2.st = p2;
    k2.tp = std::chrono::system_clock::from_time_t(getFutureTime(15, 31));
    k2.funcPtr = handleRequest;

    k3.st = p3;
    k3.tp = std::chrono::system_clock::from_time_t(getFutureTime(15, 29));
    k3.funcPtr = handleRequest;

    JobScheduler sched;
    sched.add(k1);
    sched.add(k2); 

    std::this_thread::sleep_for(std::chrono::minutes(1));
    sched.add(k3); // least due time is executed first

    std::this_thread::sleep_for(std::chrono::minutes(4));
}
