# Avapi
Avapi is a C++ library working with the [Alpha Vantage API](https://www.alphavantage.co/) for fetching historical stock data. This library is currently in a limited state and only implements 5 of the API functions. More can easily be added at any time.


# Prerequisites
To use Avapi, the following is required:
* An Alpha Vantage API key from [here](https://www.alphavantage.co/support/#api-key)
* [rapidcsv](https://github.com/d99kris/rapidcsv)
* libcurl c++ libraries


# Example Usage
## Getting intraday data
In this example, we will get intraday data for "TSLA" on a 15 minute interval. We will begin by creating an ```avapi::Quote``` object which needs a ```symbol``` and ```api_key``` to be constructed. If we saved the API key to a .txt file, Avapi provides a helper function to easily read it. 

```C++

std::string api_key = avapi::readFirstLineFromFile("api_key.txt");
std::string symbol = "TSLA";

avapi::Quote quote(symbol, api_key);
avapi::time_series series = quote.getIntradayDataSeries("15min");

```

The ```avapi::time_series``` object is a vector of pairs with each pair containing a Unix timestamp and a vector of data associated with said timestamp. For a time_series, the data is ordered as ```[open, high, low, close, volume]```.

```C++

typedef std::vector<std::pair<std::time_t, std::vector<float>>> time_series

```

We can use another helper function to see the results:

```C++

avapi::printSeries(series);

```

This gives the following output:

```
1613181600: 817 817.99 816.6 817.55 14545
1613180700: 817.05 817.5 816.54 816.98 5424
1613179800: 817 817.5 817 817.04 3234
1613178900: 818.1 818.21 817.76 817.76 5742
1613178000: 818.2 818.5 818.16 818.3 8261
...
```
## Getting weekly data
In this example, we will get weekly data for "AAPL" from the last 10 weeks.
```C++

#include "../inc/avapi.h"

int main()
{
    std::string api_key = avapi::readFirstLineFromFile("../../api.key");
    avapi::Quote quote("AAPL", api_key);
    avapi::time_series series = quote.getTimeSeries(avapi::WEEKLY, 10);
    avapi::printSeries(series);
}

```

```
1613109600: 136.03 137.877 133.692 135.37 3.44357e+08
1612504800: 133.75 137.42 130.93 136.76 4.38264e+08
1611900000: 143.07 145.09 130.21 131.96 7.16991e+08
1611295200: 127.78 139.85 126.938 139.07 4.30066e+08
1610690400: 129.19 131.45 126.86 127.14 4.81518e+08
1610085600: 133.52 133.612 126.382 132.05 6.10791e+08
1609394400: 133.99 138.789 131.72 132.69 4.39741e+08
1608789600: 125.02 134.405 123.449 131.97 4.33757e+08
1608271200: 122.6 129.58 121.54 126.655 6.21758e+08
1607666400: 122.31 125.95 120.15 122.41 4.52279e+08
```
## Getting a global quote
In this example, we will get a global quote for "GME". For a global quote, only a single time_pair object is returned with the data being ordered as
```[open, high, low, price, volume, prevClose, change, change%]```
```C++

#include "../inc/avapi.h"

int main()
{
    std::string api_key = avapi::readFirstLineFromFile("../../api.key");
    avapi::Quote quote("GME", api_key);
    avapi::time_pair pair = quote.getGlobalQuote();
    avapi::printPair(pair);
}

```
```
1613109600: 50.75 55.24 48.05 52.4 1.45733e+07 51.1 1.3 2.544
```

