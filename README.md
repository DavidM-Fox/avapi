# avapi - A C++ library for fetching historical stock data via the Alpha Vantage API
Avapi is a C++ library for working with the [Alpha Vantage](https://www.alphavantage.co/) API for fetching historical stock data. This library is currently limited to 5 different functions.



Example Usage
================
An API key from Alpha Vantage is required to utilize this library in full as well as [rapidcsv](https://github.com/d99kris/rapidcsv). Avapi currently implements 5 of the API functions within the ```avapi::Quote``` class:

* TIME_SERIES_INTRADAY as ```getIntraday(std::string interval)```
* TIME_SERIES_DAILY as ```getDaily(int last_n)```
* TIME_SERIES_WEEKLY as ```getWeekly(int last_n)```
* TIME_SERIES_MONTHLY as ```getMonthly(int last_n)```
* GLOBAL_QUOTE as ```getGlobalQuote()```

An example of how this library can be used is as follows:
1. Get an [Alpha Vantage API key](https://www.alphavantage.co/support/#api-key) and save it to a ```std::string``` variable.
```

std::string api_key = "EXAMPLEKEY";

```
2. Initialize an ```avapi::Quote``` object with the symbol "TSLA" and ```api_key```.
```

avapi::Quote quote("TSLA", api_key);

```
3. In this case, we will get intraday data on a 15 minute interval to be charted with a plotting library. Please note that the returned matrix from each of avapi's functions does not contain a time column. Alpha Vantage gives the time in the format "YYYY-mm-dd H:M:S" which is converted to a ```std::time_t timestamp```. Pushing this value to a matrix of ```float``` values would cause data loss upon conversion as ```std::time_t``` is of type ```long long```. Lets print the data as well.
```

std::vector<std::vector<float>> data_intraday = quote.getIntraday("15min");

```
```

std::cout << "open,high,low,close,volume";
for (auto &row : data_intraday) {
    for (auto &val : row) {
        std::cout << val << ',';
    }
    std::cout << '\n';
}

```
```
open,high,low,close,volume
817.05,817.5,816.54,816.98,5424,
817,817.5,817,817.04,3234,
818.1,818.21,817.76,817.76,5742,
818.2,818.5,818.16,818.3,8261,
817,818.2,816.9,818.2,11008,
816.25,817,816,817,8751.......

```
4. We want the timestamps associated with the data for our plotting purposes. Lets call ```getTimeCol()``` to return a vector of time stamps (Seconds since Unix epoch) and print it.
```

std::vector<std::time_t> timestamps = quote.getTimeCol();

```
```

for (auto &val : timestamps) {
    std::cout << val << '\n';
}

```
```

1613180700
1613179800
1613178900
1613178000
1613177100
1613176200...

```
