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
    ApiCall(std::string symbol, std::string api_key);

    std::string m_symbol;
    std::string m_apiKey;

protected:
    std::string buildApiCallUrl(const std::string &function,
                                const std::string &interval,
                                const std::string &config);
    std::string downloadCsv(const std::string &url);

private:
    static size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb,
                                      void *data);
    static const std::string m_urlBase;
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

void print(const time_series &series);
void print(const time_pair &pair);
void reverseTimeSeries(avapi::time_series &series);

extern const std::vector<float> null_vector;
extern const avapi::time_pair null_pair;
extern const avapi::time_series null_series;
} // namespace avapi
#endif // AVAPI_H