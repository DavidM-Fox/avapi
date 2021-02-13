#include "../inc/avapi.h"

int main()
{
    std::string api_key = avapi::readFirstLineFromFile("../../api.key");
    std::cout << api_key;

    avapi::Quote quote("GME", api_key);
    quote.getIntraday("15min");
    std::cout << std::endl;
    return 0;
}