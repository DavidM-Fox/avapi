#ifndef HEALTHINDEX_H
#define HEALTHINDEX_H
#include <string>
#include <vector>
#include "avapi/ApiCall.hpp"

namespace avapi {

class HealthIndex : public ApiCall {
public:
    HealthIndex();
    HealthIndex(const std::string &symbol, const std::string &key);

    std::string symbol;
    std::time_t timestamp;

    /// @brief [symbol, name, fcas rating, fcas score, developer score, market
    /// maturity score, utility score, timezone]
    std::vector<std::string> data;
    void update();
    void printData();
};

} // namespace avapi

#endif