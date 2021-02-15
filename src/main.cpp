#include "../inc/avapi.h"
#include "../inc/rapidcsv.h"

int main()
{
    std::string api_key = avapi::readFirstLineFromFile("../../api.key");
    avapi::Quote quote("TSLA", api_key);
    avapi::time_series series = quote.getIntradaySeries("15min");
    avapi::printSeries(series);
}