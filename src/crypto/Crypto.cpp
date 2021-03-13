#include "avapi/Crypto/Crypto.hpp"
#include "avapi/Crypto/HealthIndex.hpp"
#include "avapi/Crypto/Pricing.hpp"

namespace avapi {

/// @brief   avapi::Crypto Default Constructor
Crypto::Crypto() : symbol(""), api_key("") {}

/// @brief   avapi::Crypto constructor
/// @param   symbol The crypto symbol of interest
/// @param   key The Alpha Vantage API key to use
Crypto::Crypto(const std::string &symbol, const std::string &key)
    : symbol(symbol), api_key(key)
{
}

/// @brief  Crypto deconstructor
Crypto::~Crypto()
{
    if (crypto_pricing != nullptr) {
        delete crypto_pricing;
    }
    if (health_index != nullptr) {
        delete health_index;
    }
}

/// @brief Set the Alpha Vantage API key for Crypto and its components
/// @param key Alpha Vantage API key
void Crypto::setApiKey(const std::string &key)
{
    this->api_key = key;
    if (crypto_pricing != nullptr) {
        crypto_pricing->api_key = key;
    }
    if (health_index != nullptr) {
        health_index->api_key = key;
    }
}

/// @brief Set the cryptocurrency of interest for Crypto and its components
/// @param symbol cryptocurrency symbol
void Crypto::setSymbol(const std::string &symbol)
{
    this->symbol = symbol;
    if (crypto_pricing != nullptr) {
        crypto_pricing->symbol = symbol;
    }
    if (health_index != nullptr) {
        health_index->symbol = symbol;
    }
}

/// @brief Return an avapi::CryptoPricing* for this Crypto object
CryptoPricing *Crypto::Pricing()
{
    if (crypto_pricing == nullptr) {
        crypto_pricing = new CryptoPricing(symbol, api_key);
    }
    return crypto_pricing;
}

/// @brief Return an avapi::HealthIndex* for this Crypto object
HealthIndex *Crypto::Health()
{
    if (health_index == nullptr) {
        health_index = new HealthIndex(symbol, api_key);
    }
    return health_index;
}
} // namespace avapi