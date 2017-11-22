cmake_minimum_required(VERSION 2.8.12.2)
project(project2_merve_ilik_2015400006_master)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        cmake-build-debug/CMakeCache.txt
        CMakeLists.txt
        main.cpp
        Barista.h
        Barista.cpp
        Order.h
        Order.cpp
        Cashier.h
        Cashier.cpp
        )

add_executable(project2 ${SOURCE_FILES})