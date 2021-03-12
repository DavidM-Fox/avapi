#ifndef COMPANYOVERVIEW_H
#define COMPANYOVERVIEW_H
#include <string>
#include <unordered_map>
#include "avapi/ApiCall.hpp"

namespace avapi {

class CompanyOverview : public ApiCall {
public:
    CompanyOverview();
    CompanyOverview(const std::string &symbol, const std::string &api_key);

    std::string symbol;

    std::unordered_map<std::string, std::string> data;
    std::string operator[](const std::string &str) { return data[str]; }
    void Update();
};

} // namespace avapi
#endif