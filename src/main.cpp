#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <time.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

void printLocalTime() {

    time_t utc_now = time( nullptr );
    cout << utc_now << endl; // Time since Epoch

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    // now.time_since_epoch();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::cout << "Current Time "
              << std::put_time(std::localtime(&now_c), "%T") << "\n"
              << "Epoch Time "
              << duration_cast<std::chrono::seconds>(now.time_since_epoch()).count()
              << '\n';

    std::chrono::system_clock::time_point new_t = now + std::chrono::hours(1) + std::chrono::minutes(20);
    // time_t new_time_t = std::chrono::system_clock::to_time_t(new_t);
    std::cout << "Future Time "
            //   << std::put_time(std::asctime(&new_time_t), "%T") << "\n"
              << "Epoch Time "
              << duration_cast<std::chrono::seconds>(new_t.time_since_epoch()).count()
              << '\n';

}

int main() {

    std::cout << "Job Scheduler " << std::endl;
    printLocalTime();
}