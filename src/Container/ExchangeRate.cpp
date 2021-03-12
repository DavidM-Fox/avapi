#include <iostream>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include "avapi/misc.hpp"
#include "avapi/Container/ExchangeRate.hpp"

namespace avapi {

/// @brief ExchangeRate constructor
/// @param from Symbol converting from
/// @param to Symbol converting to
/// @param key An Alpha Vantage API Key
ExchangeRate::ExchangeRate(const std::string &from, const std::string &to,
                           const std::string &key)
    : from_symbol(from), to_symbol(to), ApiCall(key)
{
    Update();
}

/// @brief   Get the current exchange rate for a specific market
void ExchangeRate::Update()
{
    resetQuery();
    setFieldValue(Url::FUNCTION, "CURRENCY_EXCHANGE_RATE");
    setFieldValue(Url::FROM_CURRENCY, from_symbol);
    setFieldValue(Url::TO_CURRENCY, to_symbol);

    nlohmann::json json =
        nlohmann::json::parse(curlQuery())["Realtime Currency Exchange Rate"];

    this->timestamp = avapi::toUnixTimestamp(json["6. Last Refreshed"]);
    this->exchange_data = {std::stof(std::string(json["5. Exchange Rate"])),
                           std::stof(std::string(json["8. Bid Price"])),
                           std::stof(std::string(json["9. Ask Price"]))};
}

/// @brief print formatted avapi::ExchangeRate data
void ExchangeRate::printData()
{
    std::cout << std::string(30, '-') << '\n';
    fmt::print("|{:^28}|\n", "Exchange Rate");
    fmt::print("|{:^28}|\n", from_symbol + " -> " + to_symbol);
    std::cout << std::string(30, '-') << '\n';
    fmt::print("|{:<16}{:>12}|\n", "Timestamp:", timestamp);
    fmt::print("|{:<16}{:>12.2f}|\n", "Exchange Rate:", exchange_data[0]);
    fmt::print("|{:<16}{:>12.2f}|\n", "Bid Price:", exchange_data[1]);
    fmt::print("|{:<16}{:>12.2f}|\n", "Ask Price:", exchange_data[2]);
}

} // namespace avapi