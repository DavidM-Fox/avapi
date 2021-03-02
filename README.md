# Avapi
Avapi is a C++ library utilizing the [Alpha Vantage API](https://www.alphavantage.co/) for fetching historical stock and cryptocurrency data. This library currently implements a limited set of the API functions.

# Prerequisites
To use Avapi, the following is required:
* An Alpha Vantage API key from [here](https://www.alphavantage.co/support/#api-key)
* [rapidcsv](https://github.com/d99kris/rapidcsv)
* libcurl c++ libraries


# Example Usage
## Getting intraday data for a stock of interest
In this example, we will get the last 10 rows of intraday data for Tesla stock ("TSLA") on a 15 minute interval. We will begin by creating an ```avapi::Stock``` object with a stock ```symbol``` and ```api_key```. If we saved the API key to a .txt file, Avapi provides a helper function to easily read it.  We can then call ```Stock::getIntradaySeries("15min", 10)``` to return an ```avapi::time_series``` object containing the data we want.

The ```avapi::time_series``` object is a vector of pairs with each pair containing a Unix timestamp and a data vector. The data vector within each pair is ordered as ```[open, high, low, close, volume]```. Another helper function is used to print the series' contents.

```C++

typedef std::pair<std::time_t, std::vector<float>> time_pair
typedef std::vector<time_pair> time_series

```
```C++

std::string symbol = "TSLA";
std::string api_key = avapi::readFirstLineFromFile("api_key.txt");

avapi::Stock tsla(symbol, api_key);

avapi::time_series series = tsla.getIntradaySeries("15min", 10);
avapi::printSeries(series);

```

Output:

```
   Timestamp        Open        High         Low       Close      Volume
------------------------------------------------------------------------
  1614650400      724.30      725.44      724.01      725.25       33508
  1614649500      724.75      724.75      723.58      724.27       14049
  1614648600      723.00      724.63      723.00      724.63       13770
  1614647700      722.55      723.00      722.50      723.00        9691
  1614646800      722.50      722.60      722.50      722.60       11364
  1614645900      722.51      723.00      722.50      722.73       12976
  1614645000      722.50      723.30      721.65      723.00       25562
  1614644100      721.90      722.55      721.90      722.55       17208
  1614643200      720.50      721.80      720.30      721.49       14681
  1614642300      721.50      721.50      720.50      720.90        4320
```

## Getting daily, weekly, and monthly data for a stock of interest
For daily, weekly, and monthly data, we must specify whether or not we want an adjusted time series. For ease of viewing, the declarations for the functions used in this example are shown below


Note: If the parameter ```last_n_rows``` is defaulted to 0, the returned time_series will contain every row that Alpha Vantage supplies.
```C++

avapi::time_series Stock::getDailySeries(const bool &adjusted = false, const size_t &last_n_rows = 0);
avapi::time_series Stock::getWeeklySeries(const bool &adjusted = false, const size_t &last_n_rows = 0);
avapi::time_series Stock::getMonthlySeries(const bool &adjusted = false, const size_t &last_n_rows = 0);
void avapi::printSeries(const time_series &series, const bool &adjusted = false);
                           
```

In this first case, we will get non-adjusted daily, weekly, and monthly data for Apple stock ("AAPL").

```C++

std::string symbol = "AAPL";
std::string api_key = avapi::readFirstLineFromFile("api.key");

avapi::Stock aapl(symbol, api_key);

// Get last 10 rows of non-adjusted daily, weekly, and monthly time series
avapi::time_series daily_series = aapl.getDailySeries(false, 10);
avapi::time_series weekly_series = aapl.getWeeklySeries(false, 10);
avapi::time_series monthly_series = aapl.getMonthlySeries(false, 10);

std::cout << "Daily Series -------------------------\n\n";
avapi::printSeries(daily_series);
std::cout << '\n' << "Weekly Series ------------------------\n\n";
avapi::printSeries(weekly_series);
std::cout << '\n' << "Monthly Series-- ---------------------\n ";
avapi::printSeries(monthly_series);


```

Output:

```
Daily Series -------------------------

   Timestamp        Open        High         Low       Close      Volume
------------------------------------------------------------------------
  1614578400      123.75      127.93      122.79      127.79   116307888
  1614319200      122.59      124.85      121.20      121.26   163424672
  1614232800      124.68      126.46      120.54      120.99   144766928
  1614146400      124.94      125.56      122.23      125.35   111039904
  1614060000      123.76      126.71      118.39      125.86   158273024
  1613973600      128.01      129.72      125.60      126.00   102886920
  1613714400      130.24      130.71      128.80      129.87    87668832
  1613628000      129.20      129.99      127.41      129.71    96856752
  1613541600      131.25      132.22      129.47      130.84    97372200
  1613455200      135.49      136.01      132.79      133.19    80576320
 
Weekly Series ------------------------

   Timestamp        Open        High         Low       Close      Volume
------------------------------------------------------------------------
  1614578400      123.75      127.93      122.79      127.79   116307888
  1614319200      128.01      129.72      118.39      121.26   680391424
  1613714400      135.49      136.01      127.41      129.87   362474112
  1613109600      136.03      137.88      133.69      135.37   344357344
  1612504800      133.75      137.42      130.93      136.76   438264064
  1611900000      143.07      145.09      130.21      131.96   716990976
  1611295200      127.78      139.85      126.94      139.07   430065728
  1610690400      129.19      131.45      126.86      127.14   481518240
  1610085600      133.52      133.61      126.38      132.05   610791168
  1609394400      133.99      138.79      131.72      132.69   439740672
  
Monthly Series -----------------------

   Timestamp        Open        High         Low       Close      Volume
------------------------------------------------------------------------
  1614578400      123.75      127.93      122.79      127.79   116307888
  1614319200      133.75      137.88      118.39      121.26  1825486976
  1611900000      133.52      145.09      126.38      131.96  2239366144
  1609394400      121.01      138.79      120.01      132.69  2319687680
  1606716000      109.11      121.99      107.32      119.05  2122724352
  1604037600      117.64      125.39      107.72      108.86  2895317504
  1601445600      132.76      137.98      103.10      115.81  3886792960
  1598853600      432.80      515.14      126.00      129.04  1184207104
  1596175200      365.12      425.66      356.58      425.04   755162240
  1593496800      317.75      372.38      317.21      364.80   810900864
```

If we desire an adjusted time series, we can create one as such:

```C++

// Get last 10 rows of adjusted daily data
avapi::time_series adj_dailySeries = aapl.getDailySeries(true, 10);
avapi::printSeries(adj_dailySeries, true);

```

Output:

```
   Timestamp        open        high         low       close   adj_close      volume    dividend split_coeff
------------------------------------------------------------------------------------------------------------
  1614578400      123.75      127.93      122.79      127.79      127.79   116307888        0.00        1.00
  1614319200      122.59      124.85      121.20      121.26      121.26   163424672        0.00        1.00
  1614232800      124.68      126.46      120.54      120.99      120.99   144766928        0.00        1.00
  1614146400      124.94      125.56      122.23      125.35      125.35   111039904        0.00        1.00
  1614060000      123.76      126.71      118.39      125.86      125.86   158273024        0.00        1.00
  1613973600      128.01      129.72      125.60      126.00      126.00   102886920        0.00        1.00
  1613714400      130.24      130.71      128.80      129.87      129.87    87668832        0.00        1.00
  1613628000      129.20      129.99      127.41      129.71      129.71    96856752        0.00        1.00
  1613541600      131.25      132.22      129.47      130.84      130.84    97372200        0.00        1.00
  1613455200      135.49      136.01      132.79      133.19      133.19    80576320        0.00        1.00
```


## Getting a global quote for a stock of interest
In this example, we will get a global quote for Apple stock ("AAPL"). For a global quote, a single ```avapi::time_pair``` object is returned with the data being ordered as ```[open, high, low, price, volume, prevClose, change, change%]```. The "latestDay" column from the csv is used as the timestamp.

```C++

typedef time_pair global_quote;

```
```C++

std::string symbol = "AAPL";
std::string api_key = avapi::readFirstLineFromFile("api.key");

avapi::Stock aapl(symbol, api_key);

avapi::global_quote quote = aapl.getGlobalQuote();
avapi::printGlobalQuote(quote);

```

Output:

```
   Timestamp        Open        High         Low       Close      Volume  Prev_Close      Change     Change%
------------------------------------------------------------------------------------------------------------
  1614578400      123.75      127.93      122.79      127.79   116307888      121.26        6.53        5.39
```


## Parsing an Alpha Vantage time series csv file

If we already have a csv file from Alpha Vantage, Avapi provides a helper function to directly parse it into an ```avapi::time_series```. 

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
avapi::time_series series_a = avapi::parseCsvFile("../../data/daily_GME.csv");

// Get last 20 rows in daily_GME.csv
avapi::time_series series_b = avapi::parseCsvFile("../../data/daily_GME.csv", 20);
    
```
