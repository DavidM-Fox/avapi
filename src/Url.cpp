#include "avapi/Url.hpp"
#include <vector>
#include <string>
#include <stdexcept>

namespace avapi {

Url::Url() {}

/// @brief   Add a Url::FieldValue to m_query
/// @param   field The Url::field to be set
/// @param   value The string value
void Url::setFieldValue(const Url::Field &field, const std::string &value)
{
    // Check if field exists, then modify it
    for (auto &param : m_query) {
        if (param.field == field) {
            param.value = value;
            return;
        }
    }

    // Add new field
    m_query.push_back({field, value});
}

/// @brief  Get the specified field value
/// @return The field value as an std::string
std::string Url::getValue(const Url::Field &field)
{
    // Check if field exists, then return its value
    for (auto &pair : m_query) {
        if (pair.field == field)
            return pair.value;
    }
    return "";
}

/// @brief Construct an API url query using m_query
/// @returns An Alpha Vantage API query URL
std::string Url::buildUrlQuery()
{
    std::string url{"https://www.alphavantage.co/query?"};

    for (auto &param : m_query) {
        url += m_fieldStrings[param.field] + param.value;
    }
    return url;
}

/// @brief Vector of query field strings
const std::vector<std::string> Url::m_fieldStrings{
    "&function=",   "&symbol=",   "&interval=",      "&adjusted=",
    "&market=",     "&datatype=", "&from_currency=", "&to_currency=",
    "&outputsize=", "&apikey="};

/// @brief Alpha Vantage base url
const std::string Url::m_urlBase{"https://www.alphavantage.co/query?"};
} // namespace avapi