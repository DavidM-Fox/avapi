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

namespace API {
enum field {
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
}

// Parent class of avapi::Stock and avapi::Crypto
class ApiCall {
public:
    explicit ApiCall(const std::string &api_key);

protected:
    void setApiField(const API::field &field, const std::string &value);
    std::string getApiField(const API::field &field);
    void clearApiFields();

    std::string buildApiUrl();
    std::string queryApiUrl(const std::string &url);

private:
    std::string m_apiKey;

    std::map<std::string, std::string> m_fieldValueMap;
    static const std::vector<std::string> m_apiFieldVec;

    static size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb,
                                      void *data);
};

namespace series {
enum type { INTRADAY, DAILY, WEEKLY, MONTHLY };
}

class Stock : private ApiCall {
public:
    explicit Stock(const std::string &symbol, const std::string &api_key);

    TimeSeries getTimeSeries(const series::type &type, const bool &adjusted,
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

    TimeSeries getTimeSeries(const series::type &type,
                             const std::string &market = "USD");

    void setOutputSize(const std::string &size = "compact");

    ExchangeRate getExchangeRate(const std::string &market = "USD");

private:
    std::string m_symbol;
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
    TimeSeries(const std::vector<avapi::TimePair> &data);
    TimeSeries();

    void pushBack(const TimePair &pair);

    void setSymbol(const std::string &symbol);
    void setType(const avapi::series::type &type);
    void setAdjusted(const bool &adjusted);
    void setTitle(const std::string &title);
    void setHeaders(const std::vector<std::string> &headers);
    void reverseData();
    void printData(const size_t &count);

    std::string symbol();

    const size_t rowCount();
    const size_t colCount();

    std::vector<std::string> m_headers;

    TimePair &operator[](size_t i) { return m_data[i]; }
    friend std::ostream &operator<<(std::ostream &os, const TimeSeries &series);

private:
    std::string m_symbol;
    series::type m_type;
    std::string m_title;

    bool m_isAdjusted;

    TimePairVec m_data;
    size_t m_nRows;
    size_t m_nCols;

    std::string m_market;
};

class GlobalQuote {
public:
    GlobalQuote(const std::string &symbol, const std::time_t &timestamp,
                const std::vector<float> &data);

    const std::time_t timestamp;
    const std::string symbol;
    const float open;
    const float high;
    const float low;
    const float close;
    const float volume;
    const float close_previous;
    const float change;
    const float change_percent;
    void printData();

private:
    const std::vector<float> m_data;
    const std::vector<std::string> m_headers;
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
    void printData();
};

// Helper methods
bool stringReplace(std::string &str, const std::string &from,
                   const std::string &to);
std::string readFirstLineFromFile(const std::string &file_path);
std::time_t toUnixTimestamp(const std::string &input);

TimeSeries parseCsvFile(const std::string &file, const bool &crypto = false);
TimeSeries parseCsvString(const std::string &data, const bool &crypto = false);

void reverseTimeSeries(TimePairVec &series);
bool isJsonString(const std::string &data);

} // namespace avapi
#endif // AVAPI_H