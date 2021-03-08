#include "avapi/TimeSeries.hpp"
#include "catch.hpp"

SCENARIO("avapi::parseCsvFile()")
{
    GIVEN("A filepath to an Alpha Vantage time series csv file.")
    {
        std::string file_path = "data/daily.csv";

        WHEN("avapi::parseCsvFile() is called.")
        {
            avapi::TimeSeries series = avapi::parseCsvFile("data/daily.csv");
            THEN("The time series should contain the whole csv.")
            {
                REQUIRE(series.rowCount() == 100);
            }
        }
    }
}