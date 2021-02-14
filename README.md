# Avapi
Avapi is a C++ header-only library for working with the [Alpha Vantage API](https://www.alphavantage.co/) for fetching historical stock data. This library is currently in a limited state and only implements 5 of the API functions. More can easily be added at any time.


# Prerequisites
To use Avapi, the following is required:
* An Alpha Vantage API key from [here](https://www.alphavantage.co/support/#api-key)
* [rapidcsv](https://github.com/d99kris/rapidcsv)
* libcurl c++ libraries


# Example Usage
If we save the API key to a .txt file, we can use the helper function ```avapi::readFirstLineFromFile()``` to access it.
```
std::string api_key = avapi::readFirstLineFromFile("file_name");

```
