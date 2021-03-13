#ifndef APICALL_H
#define APICALL_H
#include <vector>
#include <string>
#include <iomanip>

namespace avapi {

class Url {
public:
    Url();

    enum class Field {
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
    std::string buildQuery();

private:
    Query m_query;
    static const std::vector<std::string> m_fieldStrings;
    static const std::string m_urlBase;
};

enum class SeriesSize { COMPACT = 0, FULL };

class ApiCall {
public:
    ApiCall();
    explicit ApiCall(const std::string &key);
    ~ApiCall();

    std::string api_key;

    void setOutputSize(const SeriesSize &size);
    std::string output_size;

    void setFieldValue(const enum Url::Field &field, const std::string &value);
    std::string getValue(const enum Url::Field &field);

    std::string curlQuery();
    void resetQuery();

private:
    Url *url = nullptr;
    static size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb,
                                      void *data);
};
} // namespace avapi
#endif