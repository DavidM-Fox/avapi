#include <iostream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include "rapidcsv.h"
#include "avapi/TimePair.hpp"
#include "avapi/TimeSeries.hpp"

namespace avapi {

/// @brief TimeSeries default constructor
TimeSeries::TimeSeries() : m_market("USD"), m_nRows(0), m_nCols(0) {}

/// @brief TimeSeries constructor
/// @param data A vector of avapi::TimePair data
TimeSeries::TimeSeries(const std::vector<avapi::TimePair> &data)
    : m_market("USD"), m_data(data)
{
    m_nRows = data.size();
    m_nCols = data[0].m_data.size();
}

/// @brief Push an avapi::TimePair into the avapi::TimeSeries
/// @param pair An avapi::TimePair to be pushed back
void TimeSeries::pushBack(const TimePair &pair) { m_data.push_back(pair); }

/// @brief   Reverses the avapi::TimeSeries' data, useful for when the data is
/// desired to be plotted
void TimeSeries::reverseData()
{
    // Data coming from Alpha Vantage is reversed (Dates are reversed)
    std::reverse(m_data.begin(), m_data.end());
}

/// @brief print formatted avapi::TimeSeries' contents
void TimeSeries::printData(const size_t &count)
{
    size_t volume_index = 0;
    size_t width = 14;
    size_t sep_count = (m_headers.size() * width) + 5;

    std::string separator(sep_count, '-');
    std::cout << separator << '\n';

    for (auto &heading : m_headers) {
        std::cout << std::setw(width) << ("|" + heading + "|");
        sep_count += width;
    }

    std::cout << '\n' << separator << '\n';

    size_t n;
    if (count > rowCount())
        size_t n = rowCount();
    else
        n = count;

    for (size_t i = 0; i < n; ++i) {
        std::cout << std::setw(width) << std::right << m_data[i].m_time;
        for (auto &value : m_data[i].m_data) {
            std::cout << std::setw(width) << std::right << std::fixed
                      << std::setprecision(2) << value;
        }
        std::cout << '\n';
    }
    std::cout << std::endl;
}

/// @brief Set the avapi::TimeSeries' column headers
/// @param headers A vector of header strings
void TimeSeries::setHeaders(const std::vector<std::string> &headers)
{
    m_headers = headers;
}

/// @brief Get the avapi::TimeSeries' row count
/// @return size_t row count
const size_t TimeSeries::rowCount() { return m_nRows; }

/// @brief Get the avapi::TimeSeries' column count
/// @return size_t column count
const size_t TimeSeries::colCount() { return m_nCols; }

/// @brief push formatted avapi::TimeSeries' contents to ostream
std::ostream &operator<<(std::ostream &os, const TimeSeries &series)
{
    size_t sep_count = 5;
    size_t volume_index = 0;
    size_t width = 15;

    for (auto &heading : series.m_headers) {
        os << std::setw(width) << heading;
        sep_count += width;
    }

    std::string separator(sep_count, '-');

    os << '\n' << separator << '\n';

    for (auto &pair : series.m_data) {
        os << std::setw(width) << std::right << pair.m_time;
        for (auto &value : pair.m_data) {
            os << std::setw(width) << std::right << std::fixed
               << std::setprecision(2) << value;
        }
        os << '\n';
    }
    return os;
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
        // 10 -> Market Cap = volume (From alpha vantage)
        // 5-8 -> Redundant USD columns
        doc.RemoveColumn(10);
        doc.RemoveColumn(8);
        doc.RemoveColumn(7);
        doc.RemoveColumn(6);
        doc.RemoveColumn(5);
    }

    for (size_t i = 0; i < n_rows; ++i) {
        std::vector<std::string> row = doc.GetRow<std::string>(i);

        // Transform vector into floats, skip timestamp column
        std::vector<float> data;
        std::transform(row.begin() + 1, row.end(), std::back_inserter(data),
                       [](std::string &value) { return std::stof(value); });

        TimePair pair(toUnixTimestamp(row[0]), data);
        series.pushBack({toUnixTimestamp(row[0]), data});
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