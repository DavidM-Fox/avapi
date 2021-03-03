#include "avapi.h"
#include <iostream>

int main()
{
    std::string symbol = "BTC";
    std::string api_key = avapi::readFirstLineFromFile("../../api.key");

    avapi::Crypto btc(symbol, api_key);

    avapi::time_series series = btc.getDailySeries();

    avapi::printSeries(series);
}