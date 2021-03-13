#ifndef CRYPTO_H
#define CRYPTO_H
#include "avapi/Crypto/HealthIndex.hpp"
#include "avapi/Crypto/Pricing.hpp"

namespace avapi {

class Crypto {
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