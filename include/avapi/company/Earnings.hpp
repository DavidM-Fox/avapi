#ifndef COMPANYEARNINGS_H
#define COMPANYEARNINGS_H
#include <string>

namespace avapi {
class CompanyEarnings {
private:
public:
    CompanyEarnings(const std::string &symbol) : symbol(symbol) {}
    ~CompanyEarnings();
    std::string symbol;
};
} // namespace avapi
#endif