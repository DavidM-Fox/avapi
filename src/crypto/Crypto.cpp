#include "avapi/Crypto/Crypto.hpp"
#include "avapi/Crypto/HealthIndex.hpp"
#include "avapi/Crypto/Pricing.hpp"

namespace avapi {

/// @brief Default constructor
Crypto::Crypto() : symbol(""), api_key(""), crypto_pricing(nullptr) {}

/// @brief Constructor
/// @param symbol: The cryptocurrency symbol of interest
/// @param key: Alpha Vantage API key
Crypto::Crypto(const std::string &symbol, const std::string &key)
    : symbol(symbol), api_key(key), crypto_pricing(nullptr)
{
}

/// @brief Crypto deconstructor
Crypto::~Crypto() {}

/// @brief Set the Alpha Vantage API key for this Crypto instance and its
/// components
/// @param key: Alpha Vantage API key
void Crypto::setApiKey(const std::string &key)
{
    this->api_key = key;
    if (crypto_pricing != nullptr) {
        crypto_pricing->api_key = key;
    }
}

/// @brief Set the cryptocurrency of interest for this Crypto instance and its
/// components
/// @param symbol: Cryptocurrency symbol
void Crypto::setSymbol(const std::string &symbol)
{
    this->symbol = symbol;
    if (crypto_pricing != nullptr) {
        crypto_pricing->symbol = symbol;
    }
}

/// @brief Return a CryptoPricing* for this instance. A new CryptoPricing
/// object is instantiated when first called
std::unique_ptr<CryptoPricing> &Crypto::pricing()
{
    if (crypto_pricing == nullptr) {
        crypto_pricing.reset(new CryptoPricing(symbol, api_key));
    }
    return crypto_pricing;
}

/// @brief Return a HealthIndex for this instance
HealthIndex Crypto::health() { return {symbol, api_key}; }
} // namespace avapi