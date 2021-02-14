cmake_minimum_required(VERSION 3.0.0)

project(avapi_test LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL CONFIG REQUIRED)

set(PROJECT_SOURCES
        src/main.cpp
        src/avapi.cpp
        inc/rapidcsv.h
        inc/avapi.h
        #inc/csv-parser.h
)

add_executable(avapi_test ${PROJECT_SOURCES})
target_link_libraries(avapi_test PRIVATE CURL::libcurl)