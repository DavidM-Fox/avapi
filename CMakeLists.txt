cmake_minimum_required(VERSION 3.0.0)

project(avapi LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

include_directories(include)
set(SRC_DIR src)
set(INC_DIR include)

set(SOURCES
        ${SRC_DIR}/ApiCall.cpp
        ${SRC_DIR}/AvapiMisc.cpp
        ${SRC_DIR}/Stock.cpp
        ${SRC_DIR}/Crypto.cpp
        ${SRC_DIR}/TimeSeries.cpp
        ${SRC_DIR}/GlobalQuote.cpp
        ${SRC_DIR}/ExchangeRate.cpp


        ${INC_DIR}/avapi.hpp
        ${INC_DIR}/rapidcsv.h
        ${INC_DIR}/avapi/ApiCall.hpp
        ${INC_DIR}/avapi/AvapiMisc.hpp
        ${INC_DIR}/avapi/Stock.hpp
        ${INC_DIR}/avapi/Crypto.hpp
        ${INC_DIR}/avapi/TimePair.hpp
        ${INC_DIR}/avapi/TimeSeries.hpp
        ${INC_DIR}/avapi/GlobalQuote.hpp
        ${INC_DIR}/avapi/ExchangeRate.hpp
)

add_executable(avapi main.cpp ${SOURCES})
target_link_libraries(avapi PRIVATE CURL::libcurl nlohmann_json nlohmann_json::nlohmann_json)

# set(TESTS
        # test/main.cpp
        # test/test01_stringReplace.cpp
        # test/test02_readFirstLineFromFile.cpp
        # test/test03_toUnixTimestamp.cpp
        # test/test04_parseCsvFile.cpp 
        # test/test05_parseCsvString.cpp
# )

# add_executable(avapi_test ${TESTS} ${PROJECT_SOURCES})
# target_link_libraries(avapi_test PRIVATE CURL::libcurl)
# 
# include(CTest)
# include(Catch)
# catch_discover_tests(avapi_test)