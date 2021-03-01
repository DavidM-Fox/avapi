#ifndef AVAPI_H
#define AVAPI_H
#include <iomanip>
#include <string>
#include <vector>

namespace avapi {

typedef std::pair<std::time_t, std::vector<float>> time_pair;
typedef std::vector<time_pair> time_series;

// Alpha Vantage API url placeholders
extern const std::string api_urlBase;
extern const std::string api_urlEnd;

extern const std::vector<std::string> api_urlFuncs;
enum api_func { INTRADAY = 0, DAILY, WEEKLY, MONTHLY };

extern const std::vector<std::string> api_urlIntervals;
enum api_interval { _1MIN = 0, _5MIN, _15MIN, _30MIN, _60MIN };

// Null objects for exception handlers
extern const std::vector<float> null_vector;
extern const avapi::time_pair null_pair;
extern const avapi::time_series null_series;

class Stock {
public:
    Stock(std::string symbol, std::string api_key);
    ~Stock();

    std::string m_symbol;
    std::string m_api_key;

    time_series getTimeSeries(const api_func &func,
                              const size_t &last_n_rows = 0,
                              const api_interval &interval = _15MIN);
    time_pair getGlobalQuote();

private:
    static size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb,
                                      void *data);
    std::string downloadCsv(const std::string &t_url);
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