#include "avapi.h"
#include "catch.hpp"

TEST_CASE("avapi::toUnixTimestamp()")
{
    REQUIRE(avapi::toUnixTimestamp("2020-08-05") == 1596607200);
    REQUIRE(avapi::toUnixTimestamp("2014-02-09") == 1391925600);
    REQUIRE(avapi::toUnixTimestamp("2003-09-17") == 1063778400);
    REQUIRE(avapi::toUnixTimestamp("2030-12-25") == 1924408800);
    REQUIRE(avapi::toUnixTimestamp("1997-01-02") == 852184800);
}