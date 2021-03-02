#ifndef AVAPI_H
#define AVAPI_H
#include <iomanip>
#include <string>
#include <vector>

namespace avapi {

typedef std::pair<std::time_t, std::vector<float>> time_pair;
typedef time_pair global_quote;
typedef std::vector<time_pair> time_series;

extern const std::vector<std::string> api_urlIntervals;

// Null objects for exception handlers
extern const std::vector<float> null_vector;
extern const avapi::time_pair null_pair;
extern const avapi::time_series null_series;

class Symbol {
public:
    Symbol(std::string symbol, std::string api_key);
    std::string m_symbol;
    std::string m_api_key;

protected:
    std::string buildApiCallUrl(const std::string &function,
                                const std::string &interval,
                                const std::string &config);
    std::string downloadCsv(const std::string &url);

private:
    static size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb,
                                      void *data);
    static const std::string api_urlBase;
};

class Stock : private Symbol {
public:
    explicit Stock(const std::string &symbol, const std::string &api_key);

    enum function { INTRADAY = 0, DAILY, WEEKLY, MONTHLY };
    enum interval { _1MIN = 0, _5MIN, _15MIN, _30MIN, _60MIN, NONE };

    time_series getTimeSeries(const Stock::function &func,
                              const Stock::interval &i,
                              const size_t &last_n_rows = 0);

    time_series getTimeSeries(const Stock::function &func,
                              const size_t &last_n_rows = 0);

    time_pair getGlobalQuote();

private:
    static const std::vector<std::string> m_functions;
    static const std::vector<std::string> m_intervals;
};

class Crypto : private Symbol {
public:
    explicit Crypto(const std::string &symbol, const std::string &api_key);
    enum function { DAILY, WEEKLY, MONTHLY };

    time_series getTimeSeries(const Crypto::function &func,
                              const size_t &last_n_rows = 0,
                              const std::string &market = "USD");

private:
    static const std::vector<std::string> m_functions;
};

// Helper methods
bool stringReplace(std::string &str, const std::string &from,
                   const std::string &to);
std::string readFirstLineFromFile(const std::string &file_path);
std::time_t toUnixTimestamp(const std::string &input);

void print(const time_series &series);
void print(const time_pair &pair);

time_series parseCsvFile(const std::string &file,
                         const size_t &last_n_rows = 0);
time_series parseCsvString(const std::string &data,
                           const size_t &last_n_rows = 0);

} // namespace avapi
#endif // AVAPI_H