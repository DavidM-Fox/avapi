#ifndef AVAPI_H
#define AVAPI_H
#include <iomanip>
#include <string>
#include <vector>

namespace avapi {

// Typedefs for Alpha Vantage API function return values
typedef std::pair<std::time_t, std::vector<float>> time_pair;
typedef time_pair global_quote;
typedef std::vector<time_pair> time_series;

// Parent class of avapi::Stock and avapi::Crypto
class ApiCall {
public:
    ApiCall();
    ApiCall(std::string symbol, std::string api_key);

    std::string m_symbol;
    std::string m_apiKey;

protected:
    enum api_function {
        TIME_SERIES_INTRADAY,
        TIME_SERIES_INTRADAY_EXTENDED,
        TIME_SERIES_DAILY,
        TIME_SERIES_DAILY_ADJUSTED,
        TIME_SERIES_WEEKLY,
        TIME_SERIES_WEEKLY_ADJUSTED,
        TIME_SERIES_MONTHLY,
        TIME_SERIES_MONTHLY_ADJUSTED,
        GLOBAL_QUOTE,
        OVERVIEW,
        CURRENCY_EXCHANGE_RATE,
        CRYPTO_RATING,
        DIGITAL_CURRENCY_DAILY,
        DIGITAL_CURRENCY_WEEKLY,
        DIGITAL_CURRENCY_MONTHLY
    };

    void setIntradayInterval(const std::string &interval);
    void setOutputSize(const std::string &size = "compact");
    void setExchange(const std::string &from, const std::string &to);
    void setMarket(const std::string &market = "USD");


    std::string buildApiUrl(const api_function &func);
    std::string queryApiUrl(const std::string &url);

private:
    static size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb,
                                      void *data);
    static const std::string m_urlBase;
    static const std::vector<std::string> m_funcStrVec;

    std::string m_outputSize;
    std::string m_interval;
    std::pair<std::string, std::string> m_exchange;
    std::string m_market;
};

class Stock : private ApiCall {
public:
    explicit Stock(const std::string &symbol, const std::string &api_key);

    time_series getIntradaySeries(const std::string &interval = "30min",
                                  const size_t &last_n_rows = 0);
    time_series getDailySeries(const bool &adjusted = false,
                               const size_t &last_n_rows = 0);
    time_series getWeeklySeries(const bool &adjusted = false,
                                const size_t &last_n_rows = 0);
    time_series getMonthlySeries(const bool &adjusted = false,
                                 const size_t &last_n_rows = 0);
    time_pair getGlobalQuote();
};

class Crypto : private ApiCall {
public:
    explicit Crypto(const std::string &symbol, const std::string &api_key);

    time_series getDailySeries(const std::string &market = "USD",
                               const size_t &last_n_rows = 0);
    time_series getWeeklySeries(const std::string &market = "USD",
                                const size_t &last_n_rows = 0);
    time_series getMonthlySeries(const std::string &market = "USD",
                                 const size_t &last_n_rows = 0);
    time_pair getExchangeRate(const std::string &market = "USD");
};

// Helper methods
bool stringReplace(std::string &str, const std::string &from,
                   const std::string &to);
std::string readFirstLineFromFile(const std::string &file_path);
std::time_t toUnixTimestamp(const std::string &input);

time_series parseCsvFile(const std::string &file,
                         const size_t &last_n_rows = 0);
time_series parseCsvString(const std::string &data,
                           const size_t &last_n_rows = 0);

void printSeries(const time_series &series, const bool &adjusted = false);
void printGlobalQuote(const time_pair &pair);
void reverseTimeSeries(avapi::time_series &series);

extern const std::vector<float> null_vector;
extern const avapi::time_pair null_pair;
extern const avapi::time_series null_series;
} // namespace avapi
#endif // AVAPI_H