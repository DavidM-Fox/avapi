#include "avapi.h"
#include "catch.hpp"

TEST_CASE("avapi::readFirstLineFromFile()", "[readFirstLineFromFile]")
{
    std::string api_key_a = avapi::readFirstLineFromFile("../ex_data/api.key");
    std::string api_key_b =
        avapi::readFirstLineFromFile("../ex_data/empty.key");

    REQUIRE(api_key_a == "THIS IS AN API KEY");
    REQUIRE(api_key_b == "");
}