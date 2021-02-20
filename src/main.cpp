#include "../inc/avapi.h"

int main()
{

    std::string api_key = avapi::readFirstLineFromFile("../../api.key");
    std::string symbol = "AAPL";

    avapi::Quote quote(symbol, api_key);
    avapi::time_series daily_series = quote.getTimeSeries(avapi::DAILY, 10);
    std::cout << "Daily Series ----------\n";
    avapi::printSeries(daily_series);
}