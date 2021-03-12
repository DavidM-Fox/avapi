#include <iostream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include "rapidcsv.h"
#include "avapi/misc.hpp"
#include "avapi/containers/TimePair.hpp"
#include "avapi/containers/TimeSeries.hpp"

namespace avapi {

/// @brief TimeSeries default constructor
TimeSeries::TimeSeries() : market("USD") {}

/// @brief TimeSeries constructor
/// @param data A vector of avapi::TimePair data
TimeSeries::TimeSeries(const std::vector<avapi::TimePair> &data)
    : market("USD"), data_series(data)
{
}

/// @brief TimeSeries copy constructor
TimeSeries::TimeSeries(const TimeSeries &series)
    : symbol(series.symbol), type(series.type), is_adjusted(series.is_adjusted),
      market(series.market), title(series.title), headers(series.headers),
      data_series(series.data_series)
{
}

/// @brief Push an avapi::TimePair into the avapi::TimeSeries
/// @param pair An avapi::TimePair to be pushed back
void TimeSeries::pushBack(const TimePair &pair) { data_series.push_back(pair); }

/// @brief   Reverses the avapi::TimeSeries' data, useful for when the data is
/// desired to be plotted
void TimeSeries::reverseData()
{
    // Data coming from Alpha Vantage is reversed (Dates are reversed)
    std::reverse(data_series.begin(), data_series.end());
}

/// @brief print formatted avapi::TimeSeries' contents
void TimeSeries::printData(const size_t &count)
{
    size_t cell_w = 14;

    size_t table_w = (headers.size() * cell_w) + headers.size() + 1;

    // Create fmt::print() format strings
    std::string title_format = "|{:^" + std::to_string(table_w - 2) + "}|";
    std::string cell_format = "|{:>" + std::to_string(cell_w) + "}";
    std::string cell_format_float = "|{:>" + std::to_string(cell_w) + ".2f}";
    std::string separator = "\n" + std::string(table_w, '-') + "\n";

    // Print Title
    std::cout << separator;
    fmt::print(title_format, this->title);
    std::cout << separator;

    // Print Headers
    for (auto &header : this->headers) {
        fmt::print(cell_format, header);
    }
    std::cout << "|";
    std::cout << separator;

    size_t n = count;
    size_t n_rows = rowCount();

    if (count > n_rows)
        n = n_rows;
    else if (count == 0)
        n = n_rows;

    // Print Data
    for (size_t i = 0; i < n; ++i) {
        fmt::print(cell_format, data_series[i].timestamp);
        for (auto &value : data_series[i].data) {
            fmt::print(cell_format_float, value);
        }
        std::cout << "|\n";
    }
}

/// @brief Set the avapi::TimeSeries' column headers
/// @param headers A vector of header strings
void TimeSeries::setHeaders(const std::vector<std::string> &headers)
{
    this->headers = headers;
}

/// @brief Get the avapi::TimeSeries' row count
/// @return size_t row count
size_t TimeSeries::rowCount() { return data_series.size(); }

/// @brief Get the avapi::TimeSeries' column count
/// @return size_t column count
size_t TimeSeries::colCount() { return data_series[0].data.size() + 1; }

/// @brief push formatted avapi::TimeSeries' contents to ostream
std::ostream &operator<<(std::ostream &os, const TimeSeries &series)
{
    size_t width = 14;
    size_t sep_count = (series.headers.size() * width) + 10;
    std::string separator(sep_count, '-');

    os << separator << '\n';

    for (auto &heading : series.headers) {
        os << std::setw(width) << heading;
    }

    os << '\n' << separator << '\n';

    for (auto &pair : series.data_series) {
        os << std::setw(width) << std::right << pair.timestamp;
        for (auto &value : pair.data) {
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

/// @brief   Returns an avapi::time_series created from a csv std::string
/// @param   data An csv std::string object
/// @param   crypto Wheter the csv data is from a crypto symbol
/// @returns avapi::TimeSeries
TimeSeries parseCsvFile(const std::string &file_path, const bool &crypto)
{
    // Create rapidcsv::Document object from csv string
    rapidcsv::Document doc(file_path);
    size_t n_rows = doc.GetRowCount();

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

} // namespace avapi