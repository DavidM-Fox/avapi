#include "avapi/company/Overview.hpp"

namespace avapi {

CompanyOverview::CompanyOverview(const std::string &symbol,
                                 const std::string &api_key)
    : symbol(symbol)
{
    api_call.api_key = api_key;
}

CompanyOverview::~CompanyOverview() {}

void CompanyOverview::setApiKey(const std::string &api_key)
{
    api_call.api_key = api_key;
}

} // namespace avapi