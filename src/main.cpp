#include "../inc/avapi.h"

int main()
{
    std::string api_key = avapi::readFirstLineFromFile("../../api.key");
    avapi::Quote quote("AAPL", api_key);
    avapi::time_series series = quote.getTimeSeries(avapi::WEEKLY, 10);
    avapi::printSeries(series);
}