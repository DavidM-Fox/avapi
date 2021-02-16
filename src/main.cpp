#include "../inc/avapi.h"

int main()
{
    std::string api_key = avapi::readFirstLineFromFile("../../api.key");
    avapi::Quote quote("GME", api_key);
    avapi::time_pair pair = quote.getGlobalQuote();
    avapi::printPair(pair);
}