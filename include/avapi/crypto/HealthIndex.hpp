#ifndef HEALTHINDEX_H
#define HEALTHINDEX_H
#include <string>
#include "avapi/ApiCall.hpp"

namespace avapi {

class HealthIndex : public ApiCall {
public:
    HealthIndex(const std::string &symbol, const std::string &key);
    ~HealthIndex();

    std::string symbol;

private:
};
} // namespace avapi

#endif