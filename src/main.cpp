#include "../inc/avapi.h"

int main()
{
    std::string api_key = avapi::readFirstLineFromFile("../../api.key");
    std::string symbol = "GME";

    avapi::Quote quote(symbol, api_key);
    avapi::time_series series = quote.getTimeSeries(avapi::WEEKLY);
    avapi::printSeries(series);
}