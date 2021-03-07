#include <iostream>
#include <iomanip>
#include "avapi/TimePair.hpp"
#include "avapi/TimeSeries.hpp"

namespace avapi {

/// @brief TimeSeries constructor
/// @param data A vector of avapi::TimePair data
TimeSeries::TimeSeries(const std::vector<avapi::TimePair> &data) : m_data(data)
{
}

/// @brief TimeSeries default constructor
TimeSeries::TimeSeries() {}

/// @brief Push an avapi::TimePair into the avapi::TimeSeries
/// @param pair An avapi::TimePair to be pushed back
void TimeSeries::pushBack(const TimePair &pair) { m_data.push_back(pair); }

/// @brief Set the avapi::TimeSeries' type
/// @param type avapi::series::type
void TimeSeries::setType(const TimeSeries::type &type) { m_type = type; }

/// @brief Set whether or not the avapi::TimeSeries has adjusted values
/// @param adjusted bool
void TimeSeries::setAdjusted(const bool &adjusted) { m_isAdjusted = adjusted; }

/// @brief Set the avapi::TimeSeries' symbol
/// @param symbol A symbol as an std::string
void TimeSeries::setSymbol(const std::string &symbol) { m_symbol = symbol; }

/// @brief Set the avapi::TimeSeries' title
/// @param symbol A title as an std::string
void TimeSeries::setTitle(const std::string &title) { m_title = title; }

/// @brief Get the avapi::TimeSeries' row count
/// @return size_t row count
const size_t TimeSeries::rowCount() { return m_nRows; }

/// @brief Get the avapi::TimeSeries' column count
/// @return size_t column count
const size_t TimeSeries::colCount() { return m_nCols; }

/// @brief Set the avapi::TimeSeries' column headers
/// @param headers A vector of header strings
void TimeSeries::setHeaders(const std::vector<std::string> &headers)
{
    m_headers = headers;
}

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

    for (size_t i = 0; i < count; ++i) {
        std::cout << std::setw(width) << std::right << m_data[i].m_time;
        for (auto &value : m_data[i].m_data) {
            std::cout << std::setw(width) << std::right << std::fixed
                      << std::setprecision(2) << value;
        }
        std::cout << '\n';
    }
    std::cout << std::endl;
}

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
} // namespace avapi