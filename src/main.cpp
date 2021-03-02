#include "avapi.h"
#include <iostream>

int main()
{
    std::string api_key = avapi::readFirstLineFromFile("../../api.key");

    std::string stock_symbol = "GME";
    std::string crypto_symbol = "BTC";

    avapi::Stock gme(stock_symbol, api_key);
    avapi::Crypto btc(crypto_symbol, api_key);

    avapi::time_series seriesA = gme.getDailySeries();
    avapi::time_series seriesB = btc.getDailySeries();

    avapi::print(seriesA);
    std::cout << "---------------\n";
    avapi::print(seriesB);
}