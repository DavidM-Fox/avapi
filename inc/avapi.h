#ifndef AVAPI_H
#define AVAPI_H
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace avapi {

// Avapi forward declarations
class TimePair;
class TimeSeries;
class GlobalQuote;
class ExchangeRate;

// Typedefs for Alpha Vantage API function return values
typedef std::vector<TimePair> TimePairVec;

// Parent class of avapi::Stock and avapi::Crypto
class ApiCall {
public:
    explicit ApiCall(const std::string &api_key);

    enum api_field {
        FUNCTION,
        SYMBOL,
        INTERVAL,
        ADJUSTED,
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

    enum function { INTRADAY, DAILY, WEEKLY, MONTHLY };
    TimeSeries getTimeSeries(const avapi::Stock::function &function,
                             const bool &adjusted,
                             const std::string &interval = "30min");

    void setOutputSize(const std::string &size = "compact");

    GlobalQuote getGlobalQuote();

private:
    std::string m_symbol;
    std::string m_outputSize;
};

class Crypto : private ApiCall {
public:
    explicit Crypto(const std::string &symbol, const std::string &api_key);

    enum function { DAILY, WEEKLY, MONTHLY };
    TimeSeries getTimeSeries(const avapi::Crypto::function &function,
                             const std::string &market = "USD");

    void setOutputSize(const std::string &size = "compact");

    ExchangeRate getExchangeRate(const std::string &market = "USD");

    std::string m_symbol;

private:
    std::string m_outputSize;
};

class TimePair {
public:
    TimePair(const std::time_t &time, const std::vector<float> &data)
        : m_time(time), m_data(data)
    {
    }

    std::time_t m_time;
    std::vector<float> m_data;
    float &operator[](size_t i) { return m_data[i]; }
};

class TimeSeries {
public:
    enum series_type { ADJUSTED, NON_ADJUSTED, CRYPTO, EMPTY };
    TimeSeries(const TimePairVec &data, const std::string &symbol,
               const series_type &type, const bool &is_adjusted,
               const bool &is_crypto, const std::string &market = "USD");
    TimeSeries(const TimePairVec &data, const std::string &symbol,
               const bool &is_crypto);
    TimeSeries(const TimePairVec &data, const series_type &type);

    void pushBack(const TimePair &pair);
    void setSymbol(const std::string &symbol);
    void setSeriesTitle(const std::string &title);
    void setHeaders(const std::vector<std::string> &headers);

    const size_t rowCount();
    const size_t colCount();
    const bool isAdjusted();
    const bool isCrypto();

    TimePair &operator[](size_t i) { return m_data[i]; }
    friend std::ostream &operator<<(std::ostream &os, const TimeSeries &series);

private:
    TimePairVec m_data;
    size_t m_nRows;
    size_t m_nCols;

    std::string m_symbol;
    series_type m_type;
    bool m_isAdjusted;
    bool m_isCrypto;
    std::string m_market;

    std::string m_title;
    std::vector<std::string> m_headers;
};

class GlobalQuote {
public:
    GlobalQuote(const std::time_t &timestamp, const std::vector<float> &data);
    const std::time_t timestamp;

    const float open;
    const float high;
    const float low;
    const float close;
    const float volume;
    const float close_previous;
    const float change;
    const float change_percent;

    const std::vector<std::string> headers;
};

class ExchangeRate {
public:
    ExchangeRate(const std::string &from, const std::string &to,
                 const std::time_t &t, const std::vector<float> &data);
    const std::string from_symbol;
    const std::string to_symbol;
    const std::time_t timestamp;
    const float exchange_rate;
    const float bid_price;
    const float ask_price;
};

// Helper methods
bool stringReplace(std::string &str, const std::string &from,
                   const std::string &to);
std::string readFirstLineFromFile(const std::string &file_path);
std::time_t toUnixTimestamp(const std::string &input);

TimeSeries parseCsvFile(const std::string &file);
TimeSeries parseCsvString(const std::string &data);

TimeSeries::series_type
discernSeriesType(const std::vector<std::string> &headers);

void reverseTimeSeries(TimePairVec &series);
bool isJsonString(const std::string &data);

} // namespace avapi
#endif // AVAPI_H