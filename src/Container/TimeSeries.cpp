#include <iostream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include "rapidcsv.h"
#include "TablePrinter.hpp"
#include "avapi/misc.hpp"
#include "avapi/Container/TimePair.hpp"
#include "avapi/Container/TimeSeries.hpp"

namespace avapi {

/// @brief Default constructor
TimeSeries::TimeSeries()
    : type(avapi::SeriesType::DAILY), is_adjusted(false), market("USD")
{
}

/// @brief Constructor
/// @param data: A vector of avapi::TimePair data
TimeSeries::TimeSeries(const std::vector<avapi::TimePair> &data)
    : type(avapi::SeriesType::DAILY), is_adjusted(false), market("USD"),
      data_series(data)
{
}

/// @brief Copy constructor
TimeSeries::TimeSeries(const TimeSeries &series)
    : symbol(series.symbol), type(series.type), is_adjusted(series.is_adjusted),
      market(series.market), title(series.title), headers(series.headers),
      data_series(series.data_series)
{
}

/// @brief Push TimePair data into the TimeSeries
/// @param pair: A TimePair to be pushed back
void TimeSeries::pushBack(const TimePair &pair) { data_series.push_back(pair); }

/// @brief Reverses the TimeSeries' data, useful for when the data is
/// desired to be plotted
void TimeSeries::reverseData()
{
    // Data coming from Alpha Vantage is reversed (Dates are reversed)
    std::reverse(data_series.begin(), data_series.end());
}

/// @brief Print formatted TimeSeries' data
/// @param count: The # of rows to print (default = 0 or all)
void TimeSeries::printData(const size_t &count)
{
    using namespace dmf::tableprinter;

    TablePrinter printer(this->title, this->headers);

    for (size_t i = 0; i < printer.columns.size(); ++i) {
        printer.columns[i].setWidth(12);
        if (i != 0) {
            printer.columns[i].data_fmt.alignment = Align::RIGHT;
            printer.columns[i].data_fmt.additional = ".2f";
        }
    }
    printer.formatHeading();
    printer.printHeading();

    size_t n = count;
    size_t n_rows = rowCount();

    if (count > n_rows || count == 0)
        n = n_rows;

    // Print Data
    for (size_t i = 0; i < n; ++i) {
        std::vector<double> data_row = {(double)data_series[i].timestamp};
        data_row.insert(data_row.end(), data_series[i].data.begin(),
                        data_series[i].data.end());
        printer.printDataRow(data_row);
    }
}

/// @brief Print formatted TimeSeries' data
/// @param count: The # of rows to print (default = 0 or all)
// void TimeSeries::printData(const size_t &count)
//{
//    size_t cell_w = 14;
//
//    size_t table_w = (headers.size() * cell_w) + headers.size() + 1;
//
//    // Create fmt::print() format strings
//    std::string title_format = "|{:^" + std::to_string(table_w - 2) + "}|";
//    std::string cell_format = "|{:>" + std::to_string(cell_w) + "}";
//    std::string cell_format_float = "|{:>" + std::to_string(cell_w) + ".2f}";
//    std::string separator = "\n" + std::string(table_w, '-') + "\n";
//
//    // Print Title
//    std::cout << separator;
//    fmt::print(title_format, this->title);
//    std::cout << separator;
//
//    // Print Headers
//    for (auto &header : this->headers) {
//        fmt::print(cell_format, header);
//    }
//    std::cout << "|";
//    std::cout << separator;
//
//    size_t n = count;
//    size_t n_rows = rowCount();
//
//    if (count > n_rows || count == 0)
//        n = n_rows;
//
//    // Print Data
//    for (size_t i = 0; i < n; ++i) {
//        fmt::print(cell_format, data_series[i].timestamp);
//        for (auto &value : data_series[i].data) {
//            fmt::print(cell_format_float, value);
//        }
//        std::cout << "|\n";
//    }
//}

/// @brief Get the TimeSeries' row count
size_t TimeSeries::rowCount() { return data_series.size(); }

/// @brief Get the TimeSeries' column count
size_t TimeSeries::colCount() { return data_series[0].data.size() + 1; }

/// @brief Push formatted TimeSeries' data to ostream
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
} // namespace avapi