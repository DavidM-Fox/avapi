#include "avapi.h"
#include <iostream>

int main()
{
    std::string api_key = avapi::readFirstLineFromFile("../../api.key");

    std::string stock = "GME";

    avapi::Stock gme(stock, api_key);

    auto quote1 =
        gme.getTimeSeries(avapi::Stock::INTRADAY, avapi::Stock::_15MIN);
    auto quote2 = gme.getTimeSeries(avapi::Stock::DAILY, 10);

    avapi::print(quote1);
}