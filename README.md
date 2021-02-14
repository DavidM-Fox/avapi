# Avapi
Avapi is a C++ header-only library for working with the [Alpha Vantage API](https://www.alphavantage.co/) for fetching historical stock data. This library is currently in a limited state and only implements 5 of the API functions. More can easily be added at any time.


# Prerequisites
To use Avapi, the following is required:
* An Alpha Vantage API key from [here](https://www.alphavantage.co/support/#api-key)
* [rapidcsv](https://github.com/d99kris/rapidcsv)
* libcurl c++ libraries


# Example Usage
In this example, we will get intraday data on a 15 minute interval for the stock "TSLA". We will begin by creating an ```avapi::Quote``` object which needs a ```symbol``` and ```api_key``` to be constructed. If we saved the API key to a .txt file, Avapi provides a helper function to easily read it. 

```C++

std::string api_key = avapi::readFirstLineFromFile("api_key.txt");
std::string symbol = "TSLA";

avapi::Quote quote(symbol, api_key);
avapi::time_series series = quote.getIntradayDataSeries("15min");

```

The ```avapi::time_series``` object is a vector of pairs with each pair containing a Unix timestamp and the data associated with it.

```C++

typedef std::vector<std::pair<std::time_t, std::vector<float>>> time_series

```

We can iterate over this object to test this library's function

```C++

for (auto &pair : series) {
    std::cout << pair.first << ": ";
    std::cout << pair.second[0] << ", ";
    std::cout << pair.second[1] << ", ";
    std::cout << pair.second[2] << ", ";
    std::cout << pair.second[3] << ", ";
    std::cout << pair.second[4] << '\n';
}
std::cout << std::endl;

```

This gives the following output:

```
1613181600: 817, 817.99, 816.6, 817.55, 14545
1613180700: 817.05, 817.5, 816.54, 816.98, 5424
1613179800: 817, 817.5, 817, 817.04, 3234
1613178900: 818.1, 818.21, 817.76, 817.76, 5742
1613178000: 818.2, 818.5, 818.16, 818.3, 8261
1613177100: 817, 818.2, 816.9, 818.2, 11008
1613176200: 816.25, 817, 816, 817, 8751
1613175300: 815.97, 816.37, 815.72, 816.37, 5832
1613174400: 815.75, 815.79, 815.7, 815.7, 4021
1613173500: 816, 816, 815.76, 815.88, 2738
1613172600: 815.3, 816, 815.12, 815.99, 16265....
```
