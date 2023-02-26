#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <time.h>
#include <chrono>

#include "jobScheduler.h"

using namespace std;
using namespace std::chrono;

void printLocalTime() {

    // time_t utc_now = time( nullptr );
    // cout << utc_now << endl; // Time since Epoch

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    // now.time_since_epoch();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::cout << "UTC Time "
              << std::put_time(std::localtime(&now_c), "%F %T") << "\t"
              << "Epoch Time "
              << duration_cast<std::chrono::seconds>(now.time_since_epoch()).count()
              << std::endl;

}

std::string handleRequest(std::shared_ptr<Payload> arg) {

    std::cout << "Job executed @ ";
    printLocalTime();
    std:: cout << "Received Inputs " 
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

    Job k1, k2;
    k1.st = p1;
    k1.funcPtr = handleRequest;

    k2.st = p2;
    k2.funcPtr = handleRequest;

    JobScheduler sched;
    std::cout << "JobScheduler Constructed " << std::endl;
    sched.add(k1, 2);
    sched.add(k2, 1); // least due time is executed first

    std::this_thread::sleep_for(std::chrono::seconds(7));
}