#ifndef APICALL_H
#define APICALL_H
#include <vector>
#include <string>
#include <iomanip>

namespace avapi {

class Url {
public:
    Url();

    enum Field {
        FUNCTION = 0,
        SYMBOL,
        INTERVAL,
        ADJUSTED,
        MARKET,
        DATA_TYPE,
        FROM_CURRENCY,
        TO_CURRENCY,
        OUTPUT_SIZE,
        API_KEY
    };

    struct FieldValue {
        Field field;
        std::string value;
    };
    typedef std::vector<FieldValue> Query;

    // set/get a Url::Field within m_query
    void setFieldValue(const Url::Field &field, const std::string &value);
    std::string getValue(const Url::Field &field);

    // Build and return the url query
    std::string buildUrlQuery();

private:
    Query m_query;
    static const std::vector<std::string> m_fieldStrings;
    static const std::string m_urlBase;
};

class ApiCall {
public:
    ApiCall();
    explicit ApiCall(const std::string &key);
    ~ApiCall();

    void setFieldValue(const enum Url::Field &field, const std::string &value);
    std::string getValue(const enum Url::Field &field);

    std::string Curl();
    void ResetUrl();

    std::string m_apiKey;
    std::string m_outputSize;

private:
    Url *m_url = nullptr;
    static size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb,
                                      void *data);
};
} // namespace avapi
#endif