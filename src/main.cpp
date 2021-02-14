#include "../inc/avapi.h"
#include "../inc/csv-parser.h"
#include <chrono>
#include <sstream>
#include <vector>

int main()
{
    std::string api_key = avapi::readFirstLineFromFile("../../api.key");
    avapi::Quote quote("GME", api_key);
    auto xy = quote.getIntraday("15min");
    auto time = quote.getTimeCol();

    for (auto &t : time) {

        std::cout << t << '\n';
    }
    std::cout << std::endl;
}