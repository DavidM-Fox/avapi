#include "avapi.hpp"

int main()
{
    std::string symbol = "BTC";
    std::string key = avapi::readApiKey("../../api.key");

    avapi::Crypto *btc = new avapi::Crypto(symbol, key);
    btc->Health()->printData();
    delete btc;
    return 0;
}