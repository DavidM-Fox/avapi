cmake_minimum_required(VERSION 3.0.0)

project(avapi LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

include_directories(include)
set(SRC_DIR src)
set(INC_DIR include)

set(PROJECT_SOURCES

        # .cpp files ---------------------------------------
        ${SRC_DIR}/ApiCall.cpp
        ${SRC_DIR}/misc.cpp

        ${SRC_DIR}/Container/AnnualEarnings.cpp
        ${SRC_DIR}/Container/ExchangeRate.cpp
        ${SRC_DIR}/Container/GlobalQuote.cpp
        ${SRC_DIR}/Container/QuarterlyEarnings.cpp
        ${SRC_DIR}/Container/TimeSeries.cpp

        ${SRC_DIR}/Crypto/Crypto.cpp
        ${SRC_DIR}/Crypto/HealthIndex.cpp
        ${SRC_DIR}/Crypto/Pricing.cpp

        ${SRC_DIR}/Company/Company.cpp
        ${SRC_DIR}/Company/Earnings.cpp
        ${SRC_DIR}/Company/Overview.cpp
        ${SRC_DIR}/Company/Stock.cpp

        # .hpp files ---------------------------------------
        ${INC_DIR}/avapi.hpp
        ${INC_DIR}/rapidcsv.h
        ${INC_DIR}/avapi/ApiCall.hpp
        ${INC_DIR}/avapi/misc.hpp

        ${INC_DIR}/avapi/Container/AnnualEarnings.hpp
        ${INC_DIR}/avapi/Container/ExchangeRate.hpp
        ${INC_DIR}/avapi/Container/GlobalQuote.hpp
        ${INC_DIR}/avapi/Container/QuarterlyEarnings.hpp
        ${INC_DIR}/avapi/Container/TimePair.hpp
        ${INC_DIR}/avapi/Container/TimeSeries.hpp
 
        ${INC_DIR}/avapi/Crypto/Crypto.hpp
        ${INC_DIR}/avapi/Crypto/HealthIndex.hpp
        ${INC_DIR}/avapi/Crypto/Pricing.hpp
 
        ${INC_DIR}/avapi/Company/Company.hpp
        ${INC_DIR}/avapi/Company/Earnings.hpp
        ${INC_DIR}/avapi/Company/Overview.hpp
        ${INC_DIR}/avapi/Company/Stock.hpp
)

add_executable(avapi main.cpp ${SOURCES})
target_link_libraries(avapi PRIVATE CURL::libcurl nlohmann_json nlohmann_json::nlohmann_json fmt::fmt)

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