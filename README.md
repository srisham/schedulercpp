# schedulercpp

## Project Overview
A time based job scheduler library using modern C++. The job can be scheduled by specifing the time (HH:MM) format, custom job function and payload (if any). When the specified time is reached, the schedule activates, causing any associated jobs to execute the job function. For more information, refer to the [example](example) code. 

## License
schedulercpp is licensed under the MIT license; see [LICENSE](LICENSE) in the source distribution for details.


## Prerequisites
Before building, you'll need to install following dependencies

    cmake

## Building
schedulercpp should work with any Linux distribution and added cross compilation support.

    Tested on:
        x86_64
        Arm
        
### Checking out the schedulercpp
To check out the schedulercpp repository:

    git clone https://github.com/srisham/schedulercpp.git
    
### Build
Use cmake tools to build the schedulercpp. Run the following commands to build the app. Set the `CMAKE_INSTALL_PREFIX` to your desired install location

    mkdir build
    cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=${PWD}/install

### Cross Compilation
In order to cross compile this module, use the cmake-tookchains file available in the [toolchain](cmake/toolchain) directory. Update the arm_toolchain.cmake file with your toolchain details. Refer the sample version in this [example](cmake/toolchain/example).
To cross-compile, run the following commands. Set the `CMAKE_TOOLCHAIN_FILE` variable to your cross toolchain file.

    mkdir build_arm
    cd build_arm
    cmake  -DCMAKE_TOOLCHAIN_FILE=<path_to_toolchain_cmake_file>/arm_toolchain.cmake ..
