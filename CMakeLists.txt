cmake_minimum_required(VERSION 2.6)
project(minihttp)


if(MSVC)
    if(NOT CMAKE_C_STANDARD_LIBRARIES MATCHES ".*lws2_32.lib.*")
        set(CMAKE_C_STANDARD_LIBRARIES "${CMAKE_C_STANDARD_LIBRARIES} ws2_32.lib" CACHE STRING "Default C libs" FORCE)
    endif()
    if(NOT CMAKE_CXX_STANDARD_LIBRARIES MATCHES ".*lws2_32.lib.*")
        set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} ws2_32.lib" CACHE STRING "Default C++ libs" FORCE)
    endif()
else()
    if(WIN32)
        if(NOT CMAKE_C_STANDARD_LIBRARIES MATCHES ".*-lws2_32.*")
            set(CMAKE_C_STANDARD_LIBRARIES "${CMAKE_C_STANDARD_LIBRARIES} -lws2_32" CACHE STRING "Default C libs" FORCE)
        endif()
        if(NOT CMAKE_CXX_STANDARD_LIBRARIES MATCHES ".*-lws2_32.*")
            set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lws2_32" CACHE STRING "Default C++ libs" FORCE)
        endif()
    endif()
    
    # non-msvc needs build type - if no build type was provided, set a default one
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug, Release, RelWithDebInfo, MinSizeRel)" FORCE)
    endif()
endif()



add_executable(htt minihttp.cpp minihttp.h main.cpp)
