#include "../inc/avapi.h"

int main()
{

    std::string api_key = avapi::readFirstLineFromFile("../../api.key");
    std::string symbol = "AAPL";
    avapi::Quote quote(symbol, api_key);
    avapi::time_series daily_series = quote.getTimeSeries(avapi::DAILY, 10);
    avapi::time_series weekly_series = quote.getTimeSeries(avapi::WEEKLY, 10);
    avapi::time_series monthly_series = quote.getTimeSeries(avapi::MONTHLY, 10);

    std::cout << "Daily Series ----------\n";
    avapi::printSeries(daily_series);
    std::cout << "Weekly Series ---------\n";
    avapi::printSeries(weekly_series);
    std::cout << "Monthly Series --------\n";
    avapi::printSeries(monthly_series);
}