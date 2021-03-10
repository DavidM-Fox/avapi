#ifndef COMPANYOVERVIEW_H
#define COMPANYOVERVIEW_H
#include <string>
#include "avapi/ApiCall.hpp"

namespace avapi {

class CompanyOverview {
public:
    CompanyOverview(const std::string &symbol, const std::string &api_key);
    ~CompanyOverview();

    void setApiKey(const std::string &api_key);

    std::string symbol;

private:
    ApiCall api_call;
};

} // namespace avapi
#endif