
# Avapi
Avapi is a C++ library utilizing the [Alpha Vantage API](https://www.alphavantage.co/) for fetching historical stock and cryptocurrency data. This library aims to implement the most popular features from the API while providing an easy to use interface for the user.

## Table of Contents
- [Avapi](#avapi)
- [Prerequisites](#prerequisites)
- [Example Usage - Stocks](#example-usage---stocks)
  * [Getting Historical Data for a Stock of Interest](#getting-historical-data-for-a-stock-of-interest)
  * [Intraday Data](#intraday-data)
  * [Daily, Weekly, and Monthly Data](#daily-weekly-and-monthly-data)
  * [Global Quote Data](#global-quote-data)
- [Example Usage - Cryptocurrencies](#example-usage---cryptocurrencies)
  * [Getting Historical Data for a Cryptocurrency of Interest](#getting-historical-data-for-a-cryptocurrency-of-interest)
- [Example Usage - Other](#example-usage---other)
  * [Parsing an Alpha Vantage time series csv file](#parsing-an-alpha-vantage-time-series-csv-file)


# Prerequisites
To use Avapi, the following is required:
* An Alpha Vantage API key from [here](https://www.alphavantage.co/support/#api-key)
* [rapidcsv](https://github.com/d99kris/rapidcsv)
* [libcurl](https://curl.se/libcurl/) C++ libraries


# Example Usage - Stocks
## Getting Historical Data for a Stock of Interest
In this example, we will explore getting historical data for Tesla stock ("TSLA"). We will begin by creating an ```avapi::Stock``` object with the  ```symbol``` "TSLA" and our Alpha Vantage ```api_key```. The ```api_key``` can be saved to a text file and then read with a provided helper function.  

```C++

std::string symbol = "TSLA";
std::string api_key = avapi::readFirstLineFromFile("api_key.txt");
avapi::Stock tsla(symbol, api_key);

```

The ```Stock``` object contains the following member methods for accessing different historical data sets:

```C++

// Intraday interval: "1min", "5min", "15min", "30min", or "60min" (default = "30min")
// If parameter last_n_rows == 0, get every available row from Alpha Vantage

time_series  getIntradaySeries(const  std::string  &interval = "30min",
			       const  size_t &last_n_rows = 0);		       
time_series  getDailySeries(const  bool  &adjusted = false,
			    const  size_t  &last_n_rows = 0);
time_series  getWeeklySeries(const  bool  &adjusted = false,
			     const  size_t  &last_n_rows = 0);
time_series  getMonthlySeries(const  bool  &adjusted = false,
			      const  size_t  &last_n_rows = 0);
time_pair  getGlobalQuote();

```

The ```time_series``` object is a vector of ```time_pairs``` with each pair containing a Unix timestamp and a data vector. 

```C++

typedef std::pair<std::time_t, std::vector<float>> time_pair
typedef std::vector<time_pair> time_series

```

The data vector within each ```time_pair``` is ordered according to the member method it is returned from:
* Intraday data:
	* ```[open, high, low, close, volume]```
* Non-adjusted daily, weekly, and monthly data:
	* ```[open, high, low, close, volume]```
* Adjusted daily, weekly, and monthly data:
	* ```[open, high, low, close, adjusted_close, volume, dividend, split_coefficient]```
* Global quote:
	* ```[open, high, low, price, volume, previous_close, change, change%]```

## Intraday Data
With our previously created ```Stock``` object (Tesla stock “TSLA”), let's look at getting **intraday**. In the following, we see two different ```time_series``` objects created for this example:

```C++

avapi::time_series series_a = tsla.getIntradaySeries();
avapi::time_series series_b = tsla.getIntradaySeries("15min", 10);

```

```series_a``` will be defaulted to an **intraday** ```time_series``` on a 30 minute interval containing every available row of data from Alpha Vantage. ```series_b``` is an **intraday** ```time_series``` on a 15 minute interval containing only the previous last 10 rows of data from today. Let's also print ```series_b``` using another helper function to view the series' contents.
```C++

avapi::printSeries(series_b);

```
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

## Daily, Weekly, and Monthly Data
Let's now look at getting daily, weekly, and monthly data for a stock of interest. We begin as always by creating an ```avapi::Stock``` object, in this case using the ```symbol``` "AAPL" for Apple stock. In the following, we see 4 different ```time_series``` created for this example:
```C++

avapi::Stock aapl("AAPL", avapi::readFirstLineFromFile("api.key"));

// Get last 10 rows of non-adjusted daily, weekly, and monthly data
avapi::time_series daily_series = aapl.getDailySeries(false, 10);
avapi::time_series weekly_series = aapl.getWeeklySeries(false, 10);
avapi::time_series monthly_series = aapl.getMonthlySeries(false, 10);

// Get last 10 rows of adjusted daily data
avapi::time_series adj_dailySeries = aapl.getDailySeries(true, 10);

// Print each time_series
std::cout << "Non-Adjusted Daily Series -------------------------\n\n";
avapi::printSeries(daily_series);
std::cout << '\n' << "Non-Adjusted Weekly Series ------------------------\n\n";
avapi::printSeries(weekly_series);
std::cout << '\n' << "Non-Adjusted Monthly Series -----------------------\n\n ";
avapi::printSeries(monthly_series);
std::cout << '\n' << "Adjusted Daily Series -----------------------------\n";
avapi::printSeries(adj_dailySeries, true);

```
```
Non-Adjusted Daily Series -------------------------

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
 
Non-Adjusted Weekly Series ------------------------

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
  
Non-Adjusted Monthly Series -----------------------

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

Adjusted Monthly Series -----------------------

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


## Global Quote Data
Using our previously created ```Stock``` object (Apple stock "AAPL"), let's look at getting a current global quote. For a global quote, a single ```time_pair``` object is returned with the data being ordered as ```[open, high, low, price, volume, previous_close, change, change%]```. The timestamp for this object is the last traded day for the stock of question.

```C++

typedef time_pair global_quote;

```
```C++

avapi::global_quote quote = aapl.getGlobalQuote();
avapi::printGlobalQuote(quote);

```
```
   Timestamp        Open        High         Low       Close      Volume  Prev_Close      Change     Change%
------------------------------------------------------------------------------------------------------------
  1614578400      123.75      127.93      122.79      127.79   116307888      121.26        6.53        5.39
```
# Example Usage - Cryptocurrencies
## Getting Historical Data for a Cryptocurrency of Interest
In this example, we will explore getting historical data for Bitcoin ("BTC"). We will begin by creating an ```avapi::Crypto``` object with the  ```symbol``` "BTC".
```C++

avapi::Crypto  btc("BTC", avapi::readFirstLineFromFile("api.key"));

```

The ```Crypto``` object contains the following member methods:

```C++
// The parameter 'market' specifies the exchange market of interest (default = "USD")
// If parameter last_n_rows == 0, get every available row from Alpha Vantage

time_series getDailySeries(const std::string &market = "USD",
			   const size_t &last_n_rows = 0);
time_series getWeeklySeries(const std::string &market = "USD",
			    const size_t &last_n_rows = 0);
time_series getMonthlySeries(const std::string &market = "USD",
			     const size_t &last_n_rows = 0);

```

Using the previously created ```Crypto``` object, lets get two different ```time_series``` of daily data:
```C++

avapi::time_series series_a = btc.getDailySeries();
avapi::time_series series_b = btc.getDailySeries("USD", 10);

```
`series_a` will be defaulted to a daily `time_series` for the market "USD" containing every available row of data from Alpha Vantage. `series_b` is a daily `time_series` for the market "USD" containing only the previous last 10 rows of data from today.

```C++

avapi::printSeries(series_b);

```
```
   Timestamp        Open        High         Low       Close      Volume
------------------------------------------------------------------------
  1614751200    48436.61    48745.13    48100.71    48578.41       48437
  1614664800    49595.76    50200.00    47047.60    48440.65       49596
  1614578400    45134.11    49790.00    44950.53    49587.03       45134
  1614492000    46103.67    46638.46    43000.00    45135.66       46104
  1614405600    46276.88    48394.00    45000.00    46106.43       46277
  1614319200    47073.73    48424.11    44106.78    46276.87       47074
  1614232800    49676.21    52041.73    46674.34    47073.73       49676
  1614146400    48891.00    51374.99    46988.69    49676.20       48891
  1614060000    54087.67    54183.59    44892.56    48891.00       54088
  1613973600    57412.35    57508.47    47622.00    54087.67       57412

```


# Example Usage - Other
## Parsing an Alpha Vantage time series csv file

If we already have a csv file from Alpha Vantage, Avapi provides a helper function to directly parse it.

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
