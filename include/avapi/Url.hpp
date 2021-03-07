#ifndef URL_H
#define URL_H
#include <string>
#include <vector>

namespace avapi {

class Url {
public:
    Url();

    enum Field {
        FUNCTION,
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
} // namespace avapi
#endif