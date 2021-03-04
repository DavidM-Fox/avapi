#ifndef AVAPI_H
#define AVAPI_H
#include <iomanip>
#include <string>
#include <vector>
#include <map>

namespace avapi {

// Typedefs for Alpha Vantage API function return values
typedef std::pair<std::time_t, std::vector<float>> time_pair;
typedef time_pair global_quote;
typedef std::vector<time_pair> time_series;

// Parent class of avapi::Stock and avapi::Crypto
class ApiCall {
public:
    explicit ApiCall(const std::string &api_key);

    enum api_field {
        FUNCTION,
        SYMBOL,
        INTERVAL,
        MARKET,
        DATA_TYPE,
        FROM_CURRENCY,
        TO_CURRENCY,
        OUTPUT_SIZE,
    };

    void setApiField(const api_field &field, const std::string &value);
    std::string getApiField(const api_field &field);
    void clearApiFields();

    std::string buildApiUrl();
    std::string queryApiUrl(const std::string &url);

    std::string m_apiKey;

private:
    std::map<std::string, std::string> m_fieldValueMap;

    static const std::string m_urlBase;
    static const std::vector<std::string> m_apiFieldVec;

    static size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb,
                                      void *data);
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

    std::string m_symbol;
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

    std::string m_symbol;
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
bool isJsonString(const std::string &data);

} // namespace avapi
#endif // AVAPI_H