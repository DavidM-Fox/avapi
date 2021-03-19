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

    std::string &symbol() { return crypto_symbol; }
    std::unique_ptr<CryptoPricing> &pricing();
    HealthIndex health();

private:
    std::string api_key;

    std::string crypto_symbol;
    std::unique_ptr<CryptoPricing> crypto_pricing;
};

} // namespace avapi
#endif