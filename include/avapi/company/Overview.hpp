#ifndef COMPANYOVERVIEW_H
#define COMPANYOVERVIEW_H
#include <string>
#include <unordered_map>
#include "avapi/ApiCall.hpp"

namespace avapi {

class CompanyOverview : public ApiCall {
public:
    CompanyOverview();
    explicit CompanyOverview(const std::string &symbol,
                             const std::string &key = "");

    std::string symbol;

    const std::string &Get(const std::string &field);
    std::string operator[](const std::string &field) { return data[field]; }
    void Update();

private:
    std::unordered_map<std::string, std::string> data;
};

} // namespace avapi
#endif