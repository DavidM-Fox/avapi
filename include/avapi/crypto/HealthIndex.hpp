#ifndef HEALTHINDEX_H
#define HEALTHINDEX_H
#include <string>
#include <vector>
#include "avapi/ApiCall.hpp"

namespace avapi {

class HealthIndex : public ApiCall {
public:
    HealthIndex(const std::string &symbol, const std::string &key);
    ~HealthIndex();

    std::string symbol;

    std::time_t &Timestamp() { return timestamp; }
    void Update();
    void printData();

private:
    std::time_t timestamp;
    std::vector<std::string> data;
};

} // namespace avapi

#endif