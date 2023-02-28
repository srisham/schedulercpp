set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(TOOLCHAIN_PREFIX /opt/toolchain/aarch64_arm/gcc-arm-11.2-2022.02-x86_64-aarch64-none-linux-gnu)
set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_PREFIX}/aarch64-none-linux-gnu/libc/)
set(CMAKE_SYSROOT ${TOOLCHAIN_PREFIX}/aarch64-none-linux-gnu/libc/)


set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}/bin/aarch64-none-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}/bin/aarch64-none-linux-gnu-g++)
set(CMAKE_CXX_FLAGS "-Werror -Wall" CACHE INTERNAL "")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)