cmake_minimum_required(VERSION 2.6)
project(minihttp)


option(MINIHTTP_USE_POLARSSL FALSE)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(EXTRA_LIBS "")

if(WIN32)
    set(EXTRA_LIBS ${EXTRA_LIBS} ws2_32)
endif()

if(MINIHTTP_USE_POLARSSL)
    add_definitions(-DMINIHTTP_USE_POLARSSL)
    find_package(polarssl)
    include_directories(${POLARSSL_INCLUDE_DIRS})
    set(EXTRA_LIBS ${EXTRA_LIBS} ${POLARSSL_LIBRARIES})
endif()


add_executable(htt minihttp.cpp minihttp.h main.cpp)
target_link_libraries(htt ${EXTRA_LIBS})
