#ifndef COMPANYOVERVIEW_H
#define COMPANYOVERVIEW_H
#include <string>
#include <unordered_map>
#include "avapi/ApiCall.hpp"

namespace avapi {

class CompanyOverview {
public:
    CompanyOverview(const std::string &symbol, const std::string &api_key);
    ~CompanyOverview();

    void setSymbol(const std::string &symbol) { this->symbol = symbol; }
    void setApiKey(const std::string &key) { api_call.api_key = key; }

    std::string symbol;
    std::unordered_map<std::string, std::string> data;
    std::string operator[](const std::string &str) { return data[str]; }

private:
    ApiCall api_call;
    void updateOverview();
};

} // namespace avapi
#endif