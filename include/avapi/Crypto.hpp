#ifndef CRYPTO_H
#define CRYPTO_H
#include <string>
#include <vector>
#include "ApiCall.hpp"
#include "Crypto/HealthIndex.hpp"
#include "Crypto/Pricing.hpp"

namespace avapi {

class Crypto : public ApiCall {
public:
    Crypto();
    explicit Crypto(const std::string &symbol, const std::string &key = "");
    ~Crypto();

    void setApiKey(const std::string &key);
    void setSymbol(const std::string &symbol);
    std::string &Symbol() { return symbol; }

    CryptoPricing *Pricing();
    HealthIndex *Health();

private:
    std::string symbol;
    std::string api_key;

    CryptoPricing *crypto_pricing = nullptr;
    HealthIndex *health_index = nullptr;
};

} // namespace avapi
#endif