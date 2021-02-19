# Avapi
Avapi is a C++ library working with the [Alpha Vantage API](https://www.alphavantage.co/) for fetching historical stock data. This library is currently in a limited state and only implements 5 of the API functions. More can easily be added at any time.


# Prerequisites
To use Avapi, the following is required:
* An Alpha Vantage API key from [here](https://www.alphavantage.co/support/#api-key)
* [rapidcsv](https://github.com/d99kris/rapidcsv)
* libcurl c++ libraries


# Example Usage
## Getting intraday data
In this example, we will get the last 30 rows of intraday data for "TSLA" on a 15 minute interval. We will begin by creating an ```avapi::Quote``` object which needs a ```symbol``` and ```api_key``` to be constructed. If we saved the API key to a .txt file, Avapi provides a helper function to easily read it.

The ```avapi::time_series``` object is a vector of pairs with each pair containing a Unix timestamp and a data vector. The data vector within each pair is ordered as ```[open, high, low, close, volume]```. Another helper function is used to print the series' contents.

```C++

typedef std::pair<std::time_t, std::vector<float>> time_pair
typedef std::vector<time_pair> time_series

std::string api_key = avapi::readFirstLineFromFile("api_key.txt");
std::string symbol = "TSLA";
avapi::Quote quote(symbol, api_key);
avapi::time_series series = quote.getTimeSeries(avapi::INTRADAY, 30, "15min");
avapi::printSeries(series);

```

Output:

```
1613674800: 46.04 48.65 45.59 46.095 3.17054e+06
1613675700: 46.05 46.1499 44.61 44.8 1.35173e+06
1613676600: 44.77 44.9499 44.2301 44.2762 635169
1613677500: 44.27 46 44.245 45.03 1.38866e+06
1613678400: 45.06 48.49 44.45 44.585 373995
1613679300: 44.555 45.15 44.2 44.2593 622516
...
```

## Getting weekly data
In this example, we will get the last 10 rows of weekly data for "AAPL".

```C++

std::string api_key = avapi::readFirstLineFromFile("../../api.key");
std::string symbol = "AAPL";
avapi::Quote quote(symbol, api_key);
avapi::time_series series = quote.getTimeSeries(avapi::WEEKLY, 10);
avapi::printSeries(series);

```

Output:

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
In this example, we will get a global quote for "GME". For a global quote, a single ```avapi::time_pair``` object is returned with the data being ordered as ```[open, high, low, price, volume, prevClose, change, change%]```. The "latestDay" column from the csv is used as the timestamp.

```C++

std::string api_key = avapi::readFirstLineFromFile("../../api.key");
std::string symbol = "GME";
avapi::Quote quote(symbol, api_key);
avapi::time_pair pair = quote.getGlobalQuote();
avapi::printPair(pair);

```

Output:

```
1613109600: 50.75 55.24 48.05 52.4 1.45733e+07 51.1 1.3 2.544
```
