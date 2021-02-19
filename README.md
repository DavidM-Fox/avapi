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

```
```C++

std::string api_key = avapi::readFirstLineFromFile("api_key.txt");
std::string symbol = "TSLA";
avapi::Quote quote(symbol, api_key);
avapi::time_series series = quote.getTimeSeries(avapi::INTRADAY, 30, "15min");
avapi::printSeries(series);

```

Output:

```
1613673900: 789.1 789.403 786.8 786.8 268074
1613674800: 787.14 787.47 785.55 786.45 358952
1613675700: 786.52 786.52 781.79 782.75 557231
1613676600: 782.9 788.13 781.86 786.45 589063
1613677500: 786.6 787.591 784.3 785.65 330470
1613678400: 785.74 786.74 784.522 785.168 307168
1613679300: 785.11 788 784.96 787.599 320274
1613680200: 787.48 789.8 786.5 788.18 375217
1613681100: 788.08 788.56 786.38 786.62 248809...
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
1612504800: 137.35 137.42 135.86 136.76 7.56938e+07
1612764000: 136.03 136.96 134.92 136.91 7.12972e+07
1612850400: 136.62 137.877 135.85 136.01 7.5987e+07...

Weekly Series ---------
1608271200: 122.6 129.58 121.54 126.655 6.21758e+08
1608789600: 125.02 134.405 123.449 131.97 4.33757e+08
1609394400: 133.99 138.789 131.72 132.69 4.39741e+08...

Monthly Series --------
1590732000: 286.25 324.24 285.85 317.94 7.0166e+08
1593496800: 317.75 372.38 317.21 364.8 8.10901e+08
1596175200: 365.12 425.66 356.58 425.04 7.55162e+08...
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


## Parsing an Alpha Vantage time series csv file

Another helper function is provided to directly parse an Alpha Vantage time series csv file. 

Example "daily_GME.csv" contents:

```
timestamp,open,high,low,close,volume
2021-02-18,48.4900,48.8700,40.6500,40.6900,23990555
2021-02-17,49.7700,51.1900,44.5600,45.9400,9147635
2021-02-16,52.6600,53.5000,49.0400,49.5100,8175030
2021-02-12,50.7500,55.2400,48.0500,52.4000,14573264
2021-02-11,50.0100,55.3200,48.2201,51.1000,13056725
2021-02-10,50.7700,62.8300,46.5500,51.2000,35943280
2021-02-09,56.6100,57.0000,46.5200,50.3100,25598598
2021-02-08,72.4100,72.6600,58.0200,60.0000,25687282
2021-02-05,54.0400,95.0000,51.0900,63.7700,81345013
2021-02-04,91.1900,91.5000,53.3300,53.5000,62427275...
```
```C++

// Get every row in daily_GME.csv
avapi::time_series series_a = avapi::parseTimeSeriesCsv("../../data/daily_GME.csv");

// Get last 20 rows in daily_GME.csv
avapi::time_series series_b = avapi::parseTimeSeriesCsv("../../data/daily_GME.csv", 20);
    
```
