cmake_minimum_required(VERSION 3.0.0)

project(avapi LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)

include_directories(inc)

set(PROJECT_SOURCES
        src/avapi.cpp
        inc/rapidcsv.h
        inc/avapi.h
)

set(TESTS
        tests/main.cpp
        tests/test01_stringReplace.cpp
        tests/test02_readFirstLineFromFile.cpp
        tests/test03_toUnixTimestamp.cpp
        tests/test04_parseCsvFile.cpp 
        tests/test05_parseCsvString.cpp
)

add_executable(avapi src/main.cpp ${PROJECT_SOURCES})
target_link_libraries(avapi PRIVATE Catch2::Catch2 CURL::libcurl)

set(TEST_DIRECTORY "tests")
file(GLOB TEST_FILES test*.cpp)

add_executable(avapi_test ${TESTS} ${PROJECT_SOURCES})
target_link_libraries(avapi_test PRIVATE CURL::libcurl)

include(CTest)
include(Catch)
catch_discover_tests(avapi_test)