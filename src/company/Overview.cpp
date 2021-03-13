#include <nlohmann/json.hpp>
#include <iostream>
#include "avapi/Company/Overview.hpp"

namespace avapi {

/// @brief Default constructor
CompanyOverview::CompanyOverview() : symbol(symbol), ApiCall("") {}

/// @brief Constructor
/// @param symbol The Company symbol e.g. "TSLA"
/// @param key Alpha Vantage API key
CompanyOverview::CompanyOverview(const std::string &symbol,
                                 const std::string &key)
    : symbol(symbol), ApiCall(key)
{
    Update();
}

/// @brief Get a CompanyOverview field value
/// @param field: The field in question
/// @return The value associated with the field
const std::string &CompanyOverview::Get(const std::string &field)
{
    return data[field];
}

/// @brief Update the Company overview
void CompanyOverview::Update()
{
    if (symbol == "" || api_key == "") {
        std::cerr << "avapi/Company/Overview.cpp: Warning: "
                     "'CompanyOverview::Update': symbol or api_key is empty. "
                     "No values were updated.\n";
        return;
    }

    resetQuery();
    setFieldValue(Url::Field::FUNCTION, "OVERVIEW");
    setFieldValue(Url::Field::SYMBOL, symbol);

    nlohmann::json json_data = nlohmann::json::parse(curlQuery());
    data.clear();
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