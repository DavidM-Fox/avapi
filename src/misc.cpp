#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>
#include "rapidcsv.h"
#include "avapi/Container/TimeSeries.hpp"
#include "avapi/misc.hpp"

namespace avapi {
/// @brief   Replaces a substring within a given string
/// @param   str The given string
/// @param   from The substring being replaced by "to"
/// @param   to The substring replacing "from"
/// @returns A success/fail bool
bool stringReplace(std::string &str, const std::string &from,
                   const std::string &to)
{
    size_t start = str.find(from);
    if (start == std::string::npos) {
        return false;
    }
    str.replace(start, from.length(), to);
    return true;
}

/// @brief   Reads the first line from a given file
/// @param   file_path File to be read's path
/// @returns first line as an std::string
std::string readFirstLineFromFile(const std::string &file_path)
{
    std::string api_key = "";
    std::ifstream file(file_path.c_str());

    if (file.is_open()) {
        std::getline(file, api_key);
        file.close();
    }
    else {

        std::exception ex(
            std::string("\"" + file_path + "\" cannot be opened").c_str());
        throw ex;
    }
    return api_key;
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