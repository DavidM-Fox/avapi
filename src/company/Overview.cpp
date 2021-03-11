#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include "avapi/company/Overview.hpp"

namespace avapi {

CompanyOverview::CompanyOverview(const std::string &symbol,
                                 const std::string &api_key)
    : symbol(symbol)
{
    api_call.api_key = api_key;
    updateOverview();
}

CompanyOverview::~CompanyOverview() {}

void CompanyOverview::updateOverview()
{
    api_call.resetQuery();
    api_call.setFieldValue(Url::FUNCTION, "OVERVIEW");
    api_call.setFieldValue(Url::SYMBOL, symbol);

    nlohmann::json json_data = nlohmann::json::parse(api_call.curlQuery());

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