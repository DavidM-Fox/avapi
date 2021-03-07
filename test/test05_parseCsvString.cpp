#include "avapi.h"
#include "catch.hpp"

SCENARIO("avapi::parseCsvString()")
{
    GIVEN("An std::string Alpha Vantage csv object.")
    {
        std::string csv_string =
            "timestamp,open,high,low,close,volume'\n'"
            "2021-02-19,130.2400,130.7100,128.8000,129.8700,87377537'\n'"
            "2021-02-18,129.2000,129.9950,127.4100,129.7100,96856748'\n'"
            "2021-02-17,131.2500,132.2200,129.4700,130.8400,97372199'\n'"
            "2021-02-16,135.4900,136.0100,132.7900,133.1900,80576316'\n'"
            "2021-02-12,134.3500,135.5300,133.6921,135.3700,60145130'\n'"
            "2021-02-11,135.9000,136.3900,133.7700,135.1300,64280029'\n'"
            "2021-02-10,136.4800,136.9900,134.4000,135.3900,72647988'\n'"
            "2021-02-09,136.6200,137.8770,135.8500,136.0100,75986989'\n'"
            "2021-02-08,136.0300,136.9600,134.9200,136.9100,71297214'\n'"
            "2021-02-05,137.3500,137.4200,135.8600,136.7600,75693830'\n'"
            "2021-02-04,136.3000,137.4000,134.5900,137.3900,84183061'\n'"
            "2021-02-03,135.7600,135.7700,133.6100,133.9400,89880937'\n'"
            "2021-02-02,135.7300,136.3100,134.6100,134.9900,82266419'\n'"
            "2021-02-01,133.7500,135.3800,130.9300,134.1400,106239823'\n'"
            "2021-01-29,135.8300,136.7400,130.2100,131.9600,177523812'\n'"
            "2021-01-28,139.5200,141.9900,136.7000,137.0900,142621128'\n'"
            "2021-01-27,143.4300,144.3000,140.4100,142.0600,140843759'\n'"
            "2021-01-26,143.6000,144.3000,141.3700,143.1600,98390555'\n'"
            "2021-01-25,143.0700,145.0900,136.5400,142.9200,157611713";

        WHEN("avapi::parseCsvString() is called without n_last_rows parameter.")
        {
            avapi::time_series series = avapi::parseCsvString(csv_string);
            THEN("The time series should contain the whole csv.")
            {
                REQUIRE(series.size() == 19);
            }
        }
        WHEN("avapi::parseCsvString() is called with n_last_rows parameter.")
        {
            avapi::time_series series = avapi::parseCsvString(csv_string, 10);
            THEN("The time series should contain only n_last_rows.")
            {
                REQUIRE(series.size() == 10);
            }
        }
        WHEN("avapi::parseCsvString() is called and n_last_rows > available "
             "rows.")
        {
            avapi::time_series series =
                avapi::parseCsvString(csv_string, 1000000);
            THEN("The time series should contain the whole csv.")
            {
                REQUIRE(series.size() == 19);
            }
        }
        WHEN("avapi::parseCsvFile() is called and n_last_rows == 0.")
        {
            avapi::time_series series = avapi::parseCsvString(csv_string, 0);
            THEN("The time series should contain the whole csv.")
            {
                REQUIRE(series.size() == 19);
            }
        }
    }
}