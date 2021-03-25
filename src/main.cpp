#include "avapi.hpp"

int main()
{
    std::string symbol = "BTC";
    std::string key = avapi::readApiKey("../../api.key");

    std::unique_ptr<avapi::Crypto> btc(new avapi::Crypto(symbol, key));
    btc->pricing()->getTimeSeries(avapi::SeriesType::DAILY, "USD").printData(3);

    return 0;
}