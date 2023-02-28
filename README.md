# schedulercpp

## Project Overview
A time based job scheduler using modern C++

## License
schedulercpp is licensed under the MIT license; see LICENSE in the source distribution for details.


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
Use cmake tools to build the schedulercpp. Run the following commands to build the app:

    mkdir build
    cd build
    cmake ..

### Cross Compilation
In order to cross compile this module, use the cmake-tookchains file available in the [toolchain](cmake/toolchain) directory. Update the arm_toolchain.cmake file with your toolchain details. Refer the sample version in this [example](cmake/toolchain/example).
To cross-compile, run the following commands:

    mkdir build_arm
    cd build_arm
    cmake  -DCMAKE_TOOLCHAIN_FILE=<path_to_toolchain_cmake_file>/arm_toolchain.cmake ..
