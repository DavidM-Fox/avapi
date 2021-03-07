#include <curl/curl.h>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include "rapidcsv.h"
#include "avapi/Url.hpp"
#include "avapi/ApiCall.hpp"
#include "avapi/TimePair.hpp"
#include "avapi/TimeSeries.hpp"

namespace avapi {

/// @brief   ApiCall Class default constructor
ApiCall::ApiCall() : m_apiKey("")
{
    m_url = new avapi::Url();
    m_url->setFieldValue(Url::API_KEY, m_apiKey);
}

/// @brief   ApiCall Class constructor
/// @param   key The Alpha Vantage API key to set
ApiCall::ApiCall(const std::string &key) : m_apiKey(key)
{
    m_url = new avapi::Url();
    m_url->setFieldValue(Url::API_KEY, m_apiKey);
}

/// @brief   ApiCall Class deconstructor
/// @param   key The Alpha Vantage API key to set
ApiCall::~ApiCall() { delete m_url; }

/// @brief   Set the API key
/// @param   key The Alpha Vantage API key to set
void ApiCall::setApiKey(const std::string &key) { m_apiKey = key; }

/// @brief   Get the instance's API Key
/// @return  The instance's Alpha Vantage API key as an std::string
const std::string &ApiCall::getApiKey() { return m_apiKey; }

/// @brief   Add a field and value to m_url
/// @param   field The Url::field to be set
/// @param   value The string value
void ApiCall::setFieldValue(const Url::Field &field, const std::string &value)
{
    m_url->setFieldValue(field, value);
}

/// @brief   Get a the specified field value within m_url
/// @param   field The Url::field
/// @returns The value corresponding to the field parameter
std::string ApiCall::getValue(const Url::Field &field)
{
    return m_url->getValue(field);
}

/// @brief   Curls m_url
/// @param   url The API query URL to be curled
/// @returns The data as an std::string
std::string ApiCall::Curl()
{
    std::string url = m_url->buildUrlQuery();
    CURL *curl;
    CURLcode res;
    std::string data;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        res = curl_easy_perform(curl);

        /* always cleanup */
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
    return data;
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

/// @brief   Returns an avapi::time_series created from a csv std::string
/// @param   data An csv std::string object
/// @param   crypto Wheter the csv data is from a crypto symbol
/// @returns avapi::TimeSeries
TimeSeries parseCsvString(const std::string &data, const bool &crypto)
{
    // Create rapidcsv::Document object from csv string
    std::stringstream sstream(data);
    rapidcsv::Document doc(sstream);
    size_t n_rows = doc.GetRowCount();

    // Test if data is really a JSON response
    if (n_rows <= 2 && isJsonString(data)) {
        nlohmann::json parser = nlohmann::json::parse(data);
        std::string error =
            "'avapi::parseCsvString': Json Response:" + parser.dump(4);
        throw std::exception(error.c_str());
    }

    // Successful parse (Cells could still be invalid...)
    TimeSeries series;
    if (crypto) {
        //// Remove useless columns
        // Volume = Market Cap (Dont know why.. free version?)
        doc.RemoveColumn(10);

        // Redundant USD columns
        doc.RemoveColumn(8);
        doc.RemoveColumn(7);
        doc.RemoveColumn(6);
        doc.RemoveColumn(5);

        for (size_t i = 0; i < n_rows; ++i) {
            std::vector<std::string> row = doc.GetRow<std::string>(i);

            // Transform vector into floats, skip timestamp column
            std::vector<float> data;
            std::transform(row.begin() + 1, row.end(), std::back_inserter(data),
                           [](std::string &value) { return std::stof(value); });
            TimePair pair(toUnixTimestamp(row[0]), data);
            series.pushBack(pair);
        }
    }
    else {
        for (size_t i = 0; i < n_rows; ++i) {
            std::vector<std::string> row = doc.GetRow<std::string>(i);

            // Transform vector into floats, skip timestamp column
            std::vector<float> data;
            std::transform(row.begin() + 1, row.end(), std::back_inserter(data),
                           [](std::string &value) { return std::stof(value); });

            TimePair pair(toUnixTimestamp(row[0]), data);
            series.pushBack(pair);
        }
    }

    std::vector<std::string> headers = doc.GetColumnNames();
    for (auto &header : headers) {

        if (header == "adjusted close" || header == "adjusted_close")
            header = "adj_close";
        else if (header == "dividend amount" || header == "dividend_amount")
            header = "dividends";
        else if (header == "split coefficient" || header == "split_coefficient")
            header = "split_coeff";
    }
    series.setHeaders(headers);
    return series;
}

/// @brief   Converts date + time string "%Y-%m-%d %H:%M:%S" to Unix Timestamp
/// (seconds since unic epoch)
/// @param   input The input string to be converted
/// @returns The time as an std::time_t unix timestamp
std::time_t toUnixTimestamp(const std::string &input)
{
    std::tm t{};
    std::istringstream ss(input);
    std::string format = "%Y-%m-%d %H:%M:%S";
    ss >> std::get_time(&t, format.c_str());
    return mktime(&t);
}

/// @brief   Test if a string is in JSON format
/// @param   data The string to be tested
/// @returns bool
bool isJsonString(const std::string &data)
{
    try {
        nlohmann::json parser = nlohmann::json::parse(data);
    }
    catch (...) {
        return false;
    }
    return true;
}

} // namespace avapi