#include <iostream>
#include <stdexcept>
#include <curl/curl.h>
#include "avapi/ApiCall.hpp"

namespace avapi {

/// @brief   Url default constructor
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
std::string Url::buildQuery()
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

/// @brief   ApiCall Class default constructor
ApiCall::ApiCall() : api_key("")
{
    url = new avapi::Url();
    url->setFieldValue(Url::API_KEY, api_key);
}

/// @brief   ApiCall Class constructor
/// @param   key The Alpha Vantage API key to set
ApiCall::ApiCall(const std::string &key) : api_key(key)
{
    url = new avapi::Url();
    url->setFieldValue(Url::API_KEY, api_key);
}

/// @brief   ApiCall Class deconstructor
/// @param   key The Alpha Vantage API key to set
ApiCall::~ApiCall()
{
    if (url != nullptr)
        delete url;
}

/// @brief   Add a field and value to url
/// @param   field The Url::field to be set
/// @param   value The string value
void ApiCall::setFieldValue(const Url::Field &field, const std::string &value)
{
    url->setFieldValue(field, value);
}

/// @brief   Get a the specified field value within url
/// @param   field The Url::field
/// @returns The value corresponding to the field parameter
std::string ApiCall::getValue(const Url::Field &field)
{
    return url->getValue(field);
}

/// @brief   Curls url
/// @param   url The API query URL to be curled
/// @returns The data as an std::string
std::string ApiCall::curlQuery()
{
    if (api_key == "") {
        throw std::exception(
            "\"avapi/ApiCall.cpp\": Alpha Vantage API key not present.");
    }

    CURL *curl;
    CURLcode res;
    std::string data;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url->buildQuery().c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        res = curl_easy_perform(curl);

        /* always cleanup */
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
    return data;
}

/// @brief   Reset the field/value queries within avapi::Url
void ApiCall::resetQuery()
{
    if (url != nullptr)
        delete url;

    url = new Url();
    url->setFieldValue(Url::API_KEY, api_key);
}

/// @brief   Callback function for CURLOPT_WRITEFUNCTION
/// @param   ptr The downloaded chunk members
/// @param   size Member memory size
/// @param   nmemb Number of members
/// @param   data Current running chunk for data appension
/// @returns The current running chunk's realsize
size_t ApiCall::WriteMemoryCallback(void *ptr, size_t size, size_t nmemb,
                                    void *data)
{
    size_t realsize = size * nmemb;

    std::string *mem = reinterpret_cast<std::string *>(data);
    mem->append(static_cast<char *>(ptr), realsize);
    return realsize;
}

} // namespace avapi