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

    std::time_t &Timestamp() { return timestamp; }
    std::string &Name() { return data[1]; }
    std::string &FcasRating() { return data[2]; }
    std::string &FcasScore() { return data[3]; }
    std::string &DeveloperScore() { return data[4]; }
    std::string &MarketMaturityScore() { return data[5]; }
    std::string &UtilityScore() { return data[6]; }
    std::string &Timezone() { return data[7]; }
    void Update();
    void printData();

private:
    std::time_t timestamp;
    std::vector<std::string> data;
};

} // namespace avapi

#endif