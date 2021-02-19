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

## Getting daily, weekly, and monthly data
In this example, we will get the last 10 rows of daily, weekly, and monthly data for "AAPL".

```C++

std::string api_key = avapi::readFirstLineFromFile("../../api.key");
std::string symbol = "AAPL";
avapi::Quote quote(symbol, api_key);
avapi::time_series daily_series = quote.getTimeSeries(avapi::DAILY, 10);
avapi::time_series weekly_series = quote.getTimeSeries(avapi::WEEKLY, 10);
avapi::time_series monthly_series = quote.getTimeSeries(avapi::MONTHLY, 10);

std::cout << "Daily Series ----------\n";
avapi::printSeries(daily_series);
std::cout << "Weekly Series ---------\n";
avapi::printSeries(weekly_series);
std::cout << "Monthly Series --------\n";
avapi::printSeries(monthly_series);

```

Output:

```
Daily Series ----------
1612418400: 91.19 91.5 53.33 53.5 6.24273e+07
1612504800: 54.04 95 51.09 63.77 8.1345e+07
1612764000: 72.41 72.66 58.02 60 2.56873e+07
...
Weekly Series ---------
1608271200: 13.34 16.3 12.14 15.63 4.88787e+07
1608789600: 15.81 22.3499 15.28 20.15 7.26563e+07
1609394400: 21.31 21.97 18.56 18.84 3.10228e+07
...
Monthly Series --------
1590732000: 5.65 6.18 3.96 4.06 5.34699e+07
1593496800: 4.12 5.53 4.02 4.34 9.99262e+07
1596175200: 4.31 4.55 3.77 4.01 5.55194e+07
...
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
