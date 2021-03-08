#include "avapi/AvapiMisc.hpp"
#include "catch.hpp"

SCENARIO("avapi::stringReplace()", "[stringReplace]")
{
    GIVEN("A string of 'This is a failed test!'")
    {
        std::string test_string = "This is a failed test!";

        WHEN("'failed' is replaced with 'successful'")
        {
            avapi::stringReplace(test_string, "failed", "successful");
            THEN("'successful' should replace 'failed'")
            {
                REQUIRE(test_string == "This is a successful test!");
            }
        }

        WHEN("'a failed' is replaced with 'another successful'")
        {
            avapi::stringReplace(test_string, "a failed", "another successful");
            THEN("'another successful' should replace 'a failed'")
            {
                REQUIRE(test_string == "This is another successful test!");
            }
        }

        WHEN("Multiple calls to stringReplace() are done in succession")
        {
            avapi::stringReplace(test_string, "failed", "really");
            avapi::stringReplace(test_string, "test", "wonderful");
            avapi::stringReplace(test_string, "wonderful",
                                 "wonderful awesome test");

            THEN("Each call should happen before the next")
            {
                REQUIRE(test_string ==
                        "This is a really wonderful awesome test!");
            }
        }
        WHEN("The 'from' parameter cannot be found")
        {
            avapi::stringReplace(test_string, "not here!",
                                 "replace with this!");
            THEN("The string should not change")
            {
                REQUIRE(test_string == "This is a failed test!");
            }
        }
        WHEN("The 'to' parameter is empty")
        {
            avapi::stringReplace(test_string, "failed", "");
            THEN("The string should not longer contain 'from'")
            {
                REQUIRE(test_string == "This is a  test!");
            }
        }
    }
}