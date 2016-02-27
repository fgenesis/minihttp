cmake_minimum_required(VERSION 2.6)
project(minihttp)


option(MINIHTTP_USE_MBEDTLS FALSE)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(EXTRA_LIBS "")

if(WIN32)
    set(EXTRA_LIBS ${EXTRA_LIBS} ws2_32)
endif()

if(MINIHTTP_USE_MBEDTLS)
    add_definitions(-DMINIHTTP_USE_MBEDTLS)
    find_package(mbedtls)
    include_directories(${MBEDTLS_INCLUDE_DIRS})
    set(EXTRA_LIBS ${EXTRA_LIBS} ${MBEDTLS_LIBRARIES})
endif()


add_executable(htt minihttp.cpp minihttp.h main.cpp)
target_link_libraries(htt ${EXTRA_LIBS})
