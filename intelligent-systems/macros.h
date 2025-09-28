#pragma once

#include <iostream>

#ifdef DEBUG_LOG_ENABLED
#define DEBUG_LOG(args) \
    do { \
        std::cout << args << std::endl; \
    } while (false)
#else
#define DEBUG_LOG(args)
#endif

#define FINDER_TIME_TEST_MS(foo, a,b,c) \
    { \
        auto start = std::chrono::high_resolution_clock::now(); \
        auto res = foo(a,b,c); \
        res.print(); \
        auto end = std::chrono::high_resolution_clock::now(); \
        std::cout << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start)).count() << " milisec" << std::endl; \
    }

#define FINDER_TIME_TEST_MCS(foo, a,b,c) \
    { \
        auto start = std::chrono::high_resolution_clock::now(); \
        auto res = foo(a,b,c); \
        res.print(); \
        auto end = std::chrono::high_resolution_clock::now(); \
        std::cout << (std::chrono::duration_cast<std::chrono::microseconds>(end - start)).count() << " microsec" << std::endl; \
    }