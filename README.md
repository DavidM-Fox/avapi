

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
  * [Getting Exchange Rate Data](#getting-exchange-rate-data)
- [Example Usage - Other](#example-usage---other)
  * [Parsing an Alpha Vantage time series csv file](#parsing-an-alpha-vantage-time-series-csv-file)


# Prerequisites
To use Avapi, the following is required:
* An Alpha Vantage API key from [here](https://www.alphavantage.co/support/#api-key)
* [d99kris/rapidcsv](https://github.com/d99kris/rapidcsv)
* [nlohmann/json](https://github.com/nlohmann/json)
* [libcurl](https://curl.se/libcurl/) C++ libraries


# Example Usage - Stocks
## Getting Historical Data for a Stock of Interest
In this example, we will explore getting historical time series data for Tesla stock ("TSLA"). We will begin by creating an ```avapi::Stock``` object with the  ```symbol``` "TSLA" and our Alpha Vantage ```api_key```. The ```api_key``` can be saved to a text file and then read with a provided helper function.  

```C++

std::string symbol = "TSLA";
std::string api_key = avapi::readFirstLineFromFile("api_key.txt");
avapi::Stock tsla(symbol, api_key);

```

The ```Stock``` object contains the following member methods for fetching historical data:

```C++

/// @brief 		Get an avapi::TimeSeries for a stock symbol of interest.
/// @param type		enum avapi::series::type
/// @param adjusted 	Whether or not the data should have adjusted values
/// @param interval 	The interval for avapi::series::INTRADAY
/// @returns 		An avapi::TimeSeries of the specified type
TimeSeries  Stock::getTimeSeries(const series::type &type, const bool &adjusted,
			         const std::string &interval = "30min");
			  
/// @brief Return the symbol's latest global quote
/// @returns the symbol's global quote as an avapi::GlobalQuote object
GlobalQuote Stock::getGlobalQuote();

```

The ```avapi::TimeSeries``` object is a class comprising of an ```std::vector``` of  ```avapi::TimePair``` objects along with other important meta data. Each ```TimePair``` object simply contains an ```std::time_t``` UNIX timestamp and an ```std::vector<float>``` data vector. The data vector can be accessed with the ```[]``` operator.

```C++

class  TimePair {
public:
	TimePair(const  std::time_t  &time, const  std::vector<float> &data)
            : m_time(time), m_data(data) {}
            
	std::time_t m_time;
	std::vector<float> m_data;
	float &operator[](size_t i) { return m_data[i]; }
};

```

The data vector within each ```TimePair``` is ordered according to the ```TimeSeries``` type:

* Adjusted/Non-Adjusted **Intraday** data:
	* ```[open, high, low, close, volume]```
* Non-adjusted **daily**, **weekly**, and **monthly** data:
	* ```[open, high, low, close, volume]```
* Adjusted **daily** data:
	* ```[open, high, low, close, adjusted_close, volume, dividend, split_coefficient]```
* Adjusted **Weekly** and **Monthly** data:
	* ```[open, high, low, close, adjusted_close, volume, dividend]```


The ```avapi::GlobalQuote``` object is a class containing the stock of interest's current global quote data. It is constructed with the stock of interest's symbol, a UNIX timestamp, and a data vector ordered: ```[open, high, low, price, volume, previous_close, change, change%]```

```C++
class  GlobalQuote {
public:
	GlobalQuote(const std::string& symbol, const std::time_t &timestamp, const std::vector<float> &data);   
	
	const std::string symbol;
        const std::time_t timestamp;
        
	const float open;
	const float high;
	const float low;
	const float close;
	const float volume;
	const float close_previous;
	const float change;
	const float change_percent;
	void printData();
};
```



## Intraday Data
With our previously created ```avapi::Stock``` object (Tesla stock “TSLA”), let's look at getting an **intraday** data set. In the following, we see an ```avapi::TimeSeries``` object created with the ```Stock::getTimeSeries()``` member method:

```C++

avapi::TimeSeries tsla_intraday = 
    tsla.getTimeSeries(avapi::series::INTRADAY, true, "15min");

```
The first parameter specifies that we want an **intraday** time series. The second parameter specifies that the data should be adjusted by historical split and dividend events. The third parameter sets the **intraday** interval to 15 minutes.  We can call the member method ```TimeSeries::printData(10)``` to print the last 10 rows of data to the console:

```C++

tsla_intraday.printData(10);

```
```
Output:
-----------------------------------------------------------------------------------------
    |timestamp|       |open|        |high|         |low|       |close|      |volume|
-----------------------------------------------------------------------------------------
    1614996000        596.97        596.97        596.00        596.25      22219.00
    1614995100        596.90        597.20        596.50        597.00      13698.00
    1614994200        597.00        597.00        595.80        596.88      14491.00
    1614993300        597.50        597.50        597.01        597.13       8955.00
    1614992400        596.08        597.94        596.08        597.13      15367.00
    1614991500        596.18        596.50        596.00        596.30       7451.00
    1614990600        597.26        597.40        596.26        596.66       7497.00
    1614989700        596.12        597.68        596.12        597.42      11905.00
    1614988800        596.00        597.00        595.75        596.52      20728.00
    1614987900        595.49        596.00        595.18        596.00      24726.00

```

The ```[]``` operator can be used to access a specific ```TimePair``` from the ```TimeSeries``` object and the data within the ```TimePair``` object:

```C++

// Print the last 10 "open" values
for (size_t  i = 0; i < 10; ++i)
	std::cout << tsla_intraday[i][0] <<  ' ';
std::cout << std::endl;

```
```
Output:
596.97 596.9 597 597.5 596.08 596.18 597.26 596.12 596 595.49
```

## Daily, Weekly, and Monthly Data
Let's now look at getting daily, weekly, and monthly data for a stock of interest. We begin as always by creating an ```avapi::Stock``` object, in this case using the ```symbol``` "AAPL" for Apple stock. In the following, we see 5 different ```TimeSeries``` created for this example:
```C++

// Create avapi::Stock object
avapi::Stock aapl("AAPL", avapi::readFirstLineFromFile("api.key"));

// Get non-adjusted daily, weekly, and monthly data
avapi::TimeSeries daily = aapl.getTimeSeries(avapi::series::DAILY, false);
avapi::TimeSeries weekly = aapl.getTimeSeries(avapi::series::WEEKLY, false);
avapi::TimeSeries monthly = aapl.getTimeSeries(avapi::series::MONTHLY, false);

// Get adjusted daily and weekly data
avapi::TimeSeries daily_adj = aapl.getTimeSeries(avapi::series::DAILY, true);
avapi::TimeSeries weekly_adj = aapl.getTimeSeries(avapi::series::WEEKLY, true);

// Print last 5 rows of data from each TimeSeries
std::cout << "Non-Adjusted Daily Series\n";
daily.printData(5);
std::cout << '\n' << "Non-Adjusted Weekly Series\n";
weekly.printData(5);
std::cout << '\n' << "Non-Adjusted Monthly Series\n";
monthly.printData(5);
std::cout << '\n' << "Adjusted Daily Series\n";
daily_adj.printData(5);
std::cout << '\n' << "Adjusted Weekly Series\n";
weekly_adj.printData(5);

```
```
Output:
Non-Adjusted Daily Series 
-----------------------------------------------------------------------------------------
   |timestamp|        |open|        |high|         |low|       |close|      |volume|
-----------------------------------------------------------------------------------------
    1614924000        120.98        121.93        117.57        121.42  153766608.00
    1614837600        121.75        123.60        118.62        120.13  178154976.00
    1614751200        124.81        125.71        121.84        122.06  112966336.00
    1614664800        128.41        128.72        125.01        125.12  102260944.00
    1614578400        123.75        127.93        122.79        127.79  116307888.00

Non-Adjusted Weekly Series
-----------------------------------------------------------------------------------------
   |timestamp|        |open|        |high|         |low|       |close|      |volume|
-----------------------------------------------------------------------------------------
    1614924000        123.75        128.72        117.57        121.42  663456768.00
    1614319200        128.01        129.72        118.39        121.26  680391424.00
    1613714400        135.49        136.01        127.41        129.87  362474112.00
    1613109600        136.03        137.88        133.69        135.37  344357344.00
    1612504800        133.75        137.42        130.93        136.76  438264064.00

Non-Adjusted Monthly Series
-----------------------------------------------------------------------------------------
   |timestamp|        |open|        |high|         |low|       |close|      |volume|
-----------------------------------------------------------------------------------------
    1614924000        123.75        128.72        117.57        121.42  663456768.00
    1614319200        128.01        129.72        118.39        121.26  680391424.00
    1613714400        135.49        136.01        127.41        129.87  362474112.00
    1613109600        136.03        137.88        133.69        135.37  344357344.00
    1612504800        133.75        137.42        130.93        136.76  438264064.00
    
```
```
Output (continued):
Adjusted Daily Series
-----------------------------------------------------------------------------------------------------------------------------------
   |timestamp|        |open|        |high|         |low|       |close|   |adj_close|      |volume|   |dividends| |split_coeff|
-----------------------------------------------------------------------------------------------------------------------------------
    1614924000        120.98        121.93        117.57        121.42        121.42  153766608.00          0.00          1.00
    1614837600        121.75        123.60        118.62        120.13        120.13  178154976.00          0.00          1.00
    1614751200        124.81        125.71        121.84        122.06        122.06  112966336.00          0.00          1.00
    1614664800        128.41        128.72        125.01        125.12        125.12  102260944.00          0.00          1.00
    1614578400        123.75        127.93        122.79        127.79        127.79  116307888.00          0.00          1.00

Adjusted Weekly Series
---------------------------------------------------------------------------------------------------------------------
   |timestamp|        |open|        |high|         |low|       |close|   |adj_close|      |volume|   |dividends|
---------------------------------------------------------------------------------------------------------------------
    1614924000        123.75        128.72        117.57        121.42        121.42  663456768.00          0.00
    1614319200        128.01        129.72        118.39        121.26        121.26  680391424.00          0.00
    1613714400        135.49        136.01        127.41        129.87        129.87  362474112.00          0.00
    1613109600        136.03        137.88        133.69        135.37        135.37  344357344.00          0.00
    1612504800        133.75        137.42        130.93        136.76        136.76  438264064.00          0.20

```


## Global Quote Data
Using our previously created ```Stock``` object (Apple stock "AAPL"), let's look at getting a current global quote. In the following, we see an ```avapi::GlobalQuote``` object created with the ```avapi::Stock``` member method ```Stock::getGlobalQuote()```. The timestamp for this object is the last traded day for the stock of question.
```C++

avapi::GlobalQuote quote = aapl.getGlobalQuote();
quote.printData();

```
```
Timestamp:       1614924000
Open:                120.98
High:                121.93
Low:                 117.57
Close:               121.42
Volume:        153766608.00
Prev_Close:          120.13
Change:                1.29
Change%:               1.07
```
# Example Usage - Cryptocurrencies
## Getting Historical Data for a Cryptocurrency of Interest
In this example, we will explore getting historical data for Bitcoin ("BTC"). We will begin by creating an ```avapi::Crypto``` object with the  ```symbol``` "BTC".
```C++

avapi::Crypto  btc("BTC", avapi::readFirstLineFromFile("api.key"));

```

The ```Crypto``` object contains the following member methods for fetching historical data.

```C++
/// @brief		Get an avapi::TimeSeries for a crypto symbol of interest.
/// @param type 	enum avapi::series::type
/// @param market 	The exchange market (default = "USD")
/// @returns 		An avapi::TimeSeries: [open,high,low,close,volume]
TimeSeries Crypto::getTimeSeries(const series::type &type,
                                 const std::string &market);

/// @brief 		Get the current exchange rate for a specific market
/// @param  market 	The exchange market (default = "USD")
/// @returns 		An avapi::ExchangeRate object: [Exchange, Bid, Ask]
ExchangeRate Crypto::getExchangeRate(const std::string &market)

```
Using the previously created ```Crypto``` object ("BTC"), we will create and print three different ```TimeSeries```:
```C++

avapi::TimeSeries daily  = btc.getTimeSeries(avapi::series::DAILY, "USD");
avapi::TimeSeries weekly  = btc.getTimeSeries(avapi::series::WEEKLY, "USD");
avapi::TimeSeries monthly  = btc.getTimeSeries(avapi::series::MONTHLY, "USD");

// Print last 5 rows of data from each TimeSeries
std::cout << "Daily Series\n";
daily.printData(5);
std::cout << '\n' << "Weekly Series\n";
weekly.printData(5);
std::cout << '\n' << "Monthly Series\n";
monthly.printData(5);

```
```
Output:
Daily Series
-----------------------------------------------------------------------------------------
   |timestamp|  |open (USD)|  |high (USD)|   |low (USD)| |close (USD)|      |volume|
-----------------------------------------------------------------------------------------
    1615010400      48746.81      49156.00      48740.65      49092.87       1258.14
    1614924000      48374.09      49448.93      46300.00      48751.71      78192.50
    1614837600      50349.37      51773.88      47500.00      48374.09      82649.72
    1614751200      48436.61      52640.00      48100.71      50349.37      81035.91
    1614664800      49595.76      50200.00      47047.60      48440.65      64221.06

Weekly Series
-----------------------------------------------------------------------------------------
   |timestamp|  |open (USD)|  |high (USD)|   |low (USD)| |close (USD)|      |volume|
-----------------------------------------------------------------------------------------
    1615010400      45134.11      52640.00      44950.53      49092.87     392443.44
    1614492000      57412.35      57508.47      43000.00      45135.66     737125.75
    1613887200      48580.47      58352.80      45570.79      57408.57     533487.81
    1613282400      38795.69      49707.43      37988.89      48577.79     664186.25
    1612677600      33092.97      40955.51      32296.16      38795.69     583442.31

Monthly Series
-----------------------------------------------------------------------------------------
   |timestamp|  |open (USD)|  |high (USD)|   |low (USD)| |close (USD)|      |volume|
-----------------------------------------------------------------------------------------
    1615010400      45134.11      52640.00      44950.53      49092.87     392443.44
    1614492000      33092.97      58352.80      32296.16      45135.66    2518242.25
    1612072800      28923.63      41950.00      28130.00      33092.98    3435639.25
    1609394400      19695.87      29300.00      17572.33      28923.63    2495281.75
    1606716000      13791.00      19863.16      13195.05      19695.87    2707065.00
```

## Getting Exchange Rate Data

Using the previously created ```Crypto``` object ("BTC"), we can get a current exchange rate for a market of choice. In the following, we see an ```avapi::ExchangeRate``` object created with the member method ```Crypto::getExchangeRate()``` while specifying the exchange market to be ```"USD"```:
```C++

avapi::ExchangeRate ex_rate = btc.getExchangeRate("USD");
ex_rate.printData();

```
```
Output:
BTC -> USD Exchange Rate
Timestamp:          1614914946
Exchange Rate:        48398.98
Bid Price:            48394.80
Ask Price             48398.59
```


# Example Usage - Other
## Parsing an Alpha Vantage time series csv file

If we already have a csv file from Alpha Vantage, Avapi provides a helper function to directly parse it:

```C++

/// @brief 	        Returns an avapi::TimeSeries created from a csv file
/// @param file		file path of the csv file to parse
/// @param crypto 	If the csv data is from a crypto symbol
/// @returns avapi::TimeSeries
TimeSeries parseCsvFile(const std::string &file_path, 
                        const bool &crypto);

```

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

avapi::TimeSeries series = avapi::parseCsvFile("daily_GME.csv", false);
series.printData(2);
    
```
```
Output:
-----------------------------------------------------------------------------------------
   |timestamp|        |open|        |high|         |low|       |close|      |volume|
-----------------------------------------------------------------------------------------
    1613628000         48.49         48.87         40.65         40.69   23990556.00
    1613541600         49.77         51.19         44.56         45.94    9147635.00

```
