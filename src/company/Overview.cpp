#include <nlohmann/json.hpp>
#include <iostream>
#include "avapi/Company/Overview.hpp"

namespace avapi {

/// @brief Default constructor
CompanyOverview::CompanyOverview() : symbol(symbol), ApiCall("") {}

/// @brief Constructor
/// @param symbol The company symbol e.g. "TSLA"
/// @param key Alpha Vantage API key
CompanyOverview::CompanyOverview(const std::string &symbol,
                                 const std::string &key)
    : symbol(symbol), ApiCall(key)
{
    Update();
}

/// @brief Get the current company overview
void CompanyOverview::Update()
{
    if (symbol == "" || api_key == "") {
        std::cerr << "avapi/Company/Overview.cpp: Warning: "
                     "'CompanyOverview::Update': symbol or api_key is empty. "
                     "No values were updated.\n";
        return;
    }

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