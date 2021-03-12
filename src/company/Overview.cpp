#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include "avapi/company/Overview.hpp"

namespace avapi {

/// @brief Constructor
/// @param symbol The company symbol e.g. "TSLA"
/// @param key Alpha Vantage API key
CompanyOverview::CompanyOverview(const std::string &symbol,
                                 const std::string &key)
    : symbol(symbol), ApiCall(key)
{
    Update();
}

CompanyOverview::~CompanyOverview() {}

/// @brief Get the current company overview
void CompanyOverview::Update()
{
    resetQuery();
    setFieldValue(Url::FUNCTION, "OVERVIEW");
    setFieldValue(Url::SYMBOL, symbol);

    nlohmann::json json_data = nlohmann::json::parse(curlQuery());

    if (json_data.is_object()) {
        auto obj = json_data.get<nlohmann::json::object_t>();
        for (auto &kvp : obj) {
            std::string key = kvp.first;
            std::string val = kvp.second.get<std::string>();
            this->data.insert({key, val});
        }
    }
}

} // namespace avapi