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