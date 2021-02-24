#include "avapi.h"
#include "catch.hpp"

SCENARIO("avapi::parseCsvFile()")
{
    GIVEN("A filepath to an Alpha Vantage time series csv file.")
    {
        std::string file_path = "../ex_data/daily.csv";

        WHEN("avapi::parseCsvFile() is called without n_last_rows parameter.")
        {
            avapi::time_series series =
                avapi::parseCsvFile("../ex_data/daily.csv");
            THEN("The time series should contain the whole csv.")
            {
                REQUIRE(series.size() == 100);
            }
        }
        WHEN("avapi::parseCsvFile() is called with n_last_rows parameter.")
        {
            avapi::time_series series =
                avapi::parseCsvFile("../ex_data/daily.csv", 20);
            THEN("The time series should contain only n_last_rows.")
            {
                REQUIRE(series.size() == 20);
            }
        }
        WHEN(
            "avapi::parseCsvFile() is called and n_last_rows > available rows.")
        {
            avapi::time_series series =
                avapi::parseCsvFile("../ex_data/daily.csv", 1000000);
            THEN("The time series should contain the whole csv.")
            {
                REQUIRE(series.size() == 100);
            }
        }
        WHEN("avapi::parseCsvFile() is called and n_last_rows == 0.")
        {
            avapi::time_series series =
                avapi::parseCsvFile("../ex_data/daily.csv", 0);
            THEN("The time series should contain the whole csv.")
            {
                REQUIRE(series.size() == 100);
            }
        }
    }
}