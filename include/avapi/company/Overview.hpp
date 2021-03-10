#ifndef COMPANYOVERVIEW_H
#define COMPANYOVERVIEW_H
#include <string>

namespace avapi {
class CompanyOverview {
public:
    CompanyOverview(const std::string &symbol) : symbol(symbol) {}
    ~CompanyOverview();

    std::string symbol;
};
} // namespace avapi
#endif