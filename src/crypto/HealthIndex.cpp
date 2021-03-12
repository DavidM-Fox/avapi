#include "avapi/crypto/HealthIndex.hpp"

namespace avapi {

HealthIndex::HealthIndex(const std::string &symbol, const std::string &key)
    : symbol(symbol), ApiCall(key)
{
}
HealthIndex::~HealthIndex() {}

} // namespace avapi
