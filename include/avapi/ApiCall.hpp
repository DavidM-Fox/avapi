#ifndef APICALL_H
#define APICALL_H
#include "Url.hpp"
#include <iomanip>

namespace avapi {

class TimeSeries;

class ApiCall {
public:
    // Default Constructor
    ApiCall();

    // Construct with an Alpha Vantage API Key
    explicit ApiCall(const std::string &key);
    ~ApiCall();

    // set/get m_apiKey
    void setApiKey(const std::string &key);
    const std::string &getApiKey();

    // set/get a Url::Field value within m_url
    void setFieldValue(const Url::Field &field, const std::string &value);
    std::string getValue(const Url::Field &field);

    // Curl the url query within m_url
    std::string Curl();

private:
    std::string m_apiKey;
    Url *m_url = nullptr;
    static size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb,
                                      void *data);
};

TimeSeries parseCsvFile(const std::string &file, const bool &crypto = false);
TimeSeries parseCsvString(const std::string &data, const bool &crypto = false);

std::time_t toUnixTimestamp(const std::string &input);
bool isJsonString(const std::string &data);

} // namespace avapi
#endif