#include "../inc/avapi.h"

int main()
{
    std::string date_0 = "2020-08-05";
    std::string date_1 = "2014-02-09";
    std::string date_2 = "2003-09-17";
    std::string date_3 = "2030-12-25";
    std::string date_4 = "1997-01-02";

    std::cout << avapi::toUnixTimestamp(date_0) << '\n';
    std::cout << avapi::toUnixTimestamp(date_1) << '\n';
    std::cout << avapi::toUnixTimestamp(date_2) << '\n';
    std::cout << avapi::toUnixTimestamp(date_3) << '\n';
    std::cout << avapi::toUnixTimestamp(date_4) << std::endl;
}