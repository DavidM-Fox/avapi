#include "avapi.hpp"
#include <iostream>

int main()
{
    std::string key = avapi::readFirstLineFromFile("../../api.key");
    std::string symbol = "GME";

    avapi::Stock gme(symbol, key);

    avapi::TimeSeries series =
        gme.getTimeSeries(avapi::SeriesType::DAILY, false);

    series.printData(10);
}