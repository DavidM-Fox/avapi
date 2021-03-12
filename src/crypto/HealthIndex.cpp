#include <iostream>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include "avapi/misc.hpp"
#include "avapi/Crypto/HealthIndex.hpp"

namespace avapi {

HealthIndex::HealthIndex(const std::string &symbol, const std::string &key)
    : symbol(symbol), ApiCall(key)
{
    Update();
}
HealthIndex::~HealthIndex() {}

void HealthIndex::Update()
{
    resetQuery();
    setFieldValue(Url::FUNCTION, "CRYPTO_RATING");
    setFieldValue(Url::SYMBOL, symbol);

    nlohmann::json json =
        nlohmann::json::parse(curlQuery())["Crypto Rating (FCAS)"];

    // Skip "1. symbol" and "8. last refreshed"
    data.push_back(json["1. symbol"]);
    data.push_back(json["2. name"]);
    data.push_back(json["3. fcas rating"]);
    data.push_back(json["4. fcas score"]);
    data.push_back(json["5. developer score"]);
    data.push_back(json["6. market maturity score"]);
    data.push_back(json["7. utility score"]);
    timestamp = avapi::toUnixTimestamp(json["8. last refreshed"]);
    data.push_back(json["9. timezone"]);
}

void HealthIndex::printData()
{
    std::cout << std::string(40, '-') << '\n';
    fmt::print("|{:^38}|\n", "Health Index");
    fmt::print("|{:^38}|\n", data[0]);
    std::cout << std::string(40, '-') << '\n';

    fmt::print("|{:<26}{:>12}|\n", "Name:", data[1]);
    fmt::print("|{:<26}{:>12}|\n", "Fcas Rating:", data[2]);
    fmt::print("|{:<26}{:>12}|\n", "Fcas Score:", data[3]);
    fmt::print("|{:<26}{:>12}|\n", "Developer score:", data[4]);
    fmt::print("|{:<26}{:>12}|\n", "Market Maturity Score:", data[5]);
    fmt::print("|{:<26}{:>12}|\n", "Utility Score:", data[6]);
    fmt::print("|{:<26}{:>12}|\n", "Last refreshed:", (timestamp));
    fmt::print("|{:<26}{:>12}|\n", "Timezone:", data[7]);
}

} // namespace avapi
