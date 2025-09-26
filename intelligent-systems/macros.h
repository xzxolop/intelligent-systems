#pragma once

#include <iostream>

#ifndef DEBUG_LOG_DISABLED
#define DEBUG_LOG(args) \
    do { \
        std::cout << args << std::endl; \
    } while (false)
#else
#define DEBUG_LOG(args)
#endif