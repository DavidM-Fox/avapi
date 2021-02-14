#include "../inc/avapi.h"
#include "../inc/rapidcsv.h"

int main()
{
    std::string api_key = avapi::readFirstLineFromFile("../../api.key");
    avapi::Quote quote("TSLA", api_key);
    avapi::time_series series = quote.getIntradayDataSeries("15min");

    for (auto &pair : series) {
        std::cout << pair.first << ": ";
        std::cout << pair.second[0] << ", ";
        std::cout << pair.second[1] << ", ";
        std::cout << pair.second[2] << ", ";
        std::cout << pair.second[3] << ", ";
        std::cout << pair.second[4] << '\n';
    }
    std::cout << std::endl;
}