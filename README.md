


# Avapi
Avapi is a C++ library utilizing the [Alpha Vantage API](https://www.alphavantage.co/) for fetching historical stock and cryptocurrency data. This library aims to implement the most popular features from the API while providing an easy to use interface for the user.

## Table of Contents
- [Avapi](#avapi)
- [Prerequisites](#prerequisites)
- [Example Usage](#example-usage)
  * [Company Information and Historical Stock Data](#company-information-and-historical-stock-data)
  * [Cryptocurrency Historical Pricing Data](#cryptocurrency-historical-pricing-data)
  * [Parsing an Alpha Vantage time series csv file](#parsing-an-alpha-vantage-time-series-csv-file)


# Prerequisites
To use Avapi, the following is required:
* An Alpha Vantage API key from [here](https://www.alphavantage.co/support/#api-key)
* [<d99kris/rapidcsv>](https://github.com/d99kris/rapidcsv)
* [<nlohmann/json>](https://github.com/nlohmann/json)
* [<fmtlib/fmt>](https://github.com/fmtlib/fmt)
* [libcurl](https://curl.se/libcurl/) C++ libraries


# Example Usage

## Company Information and Historical Stock Data
Avapi provides a simple interface for accessing company information and stock data for a symbol of interest. For the following examples, we will explore the available functions of the ```avapi::Company``` class with Tesla ("TSLA").  With the  ```symbol``` "TSLA" and our Alpha Vantage API ```key```, lets create a new ```Company``` object. Note: The ```key``` can be saved to a text file and then read with a provided helper function.

```C++

std::string symbol = "TSLA";
std::string key = avapi::readFirstLineFromFile("api.key");
avapi::Company *tsla = new avapi::Company(symbol, key);

```
The ```Company``` class has three component classes with corresponding methods to access pointers to them:
 - avapi::CompanyEarnings - Earnings()
 - avapi::CompanyOverview - Overview()
 - avapi::CompanyStock - Stock()
---
**Company Information - Annual and Quarterly Earnings:**

The following shows us accessing the annual and quarterly earnings for Tesla and then printing them to console. (The displayed output only shows the first 3 rows)
```C++

auto tsla_annual = tsla->Earnings()->Annual();
auto tsla_quarterly = tsla->Earnings()->Quarterly();

tsla_annual->printData();
tsla_quarterly->printData();

```
```
Output:
--------------------------------------
|        TSLA Annual Earnings        |
| Fiscal Date Ending | Reported EPS  |
--------------------------------------
|     2020-12-31     |           2.22|
|     2019-12-31     |          -0.02|
|     2018-12-31     |          -0.32|...
```
```
Output:
------------------------------------------------------------------------------------------------------
|                                      TSLA Quarterly Earnings                                       |
| Fiscal Date Ending | Reported Date | Reported EPS  | Estimated EPS |   Surprise    |  Surprise %   |
------------------------------------------------------------------------------------------------------
|     2020-12-31     |  2021-01-27   |           0.80|           1.05|          -0.25|         -23.91|
|     2020-09-30     |  2020-10-21   |           0.76|           0.60|           0.16|          25.79|
|     2020-06-30     |  2020-07-22   |           0.44|          -0.03|           0.47|        1458.26|...
```
---
**Company Information - Overview:**

The ```avapi::CompanyOverview```class has an ```std::unordered_map``` containing the whole JSON response from Alpha Vantage's [company overview](https://www.alphavantage.co/documentation/#company-overview) function. The full list of available data fields can be viewed at the following [JSON demo](https://www.alphavantage.co/query?function=OVERVIEW&symbol=IBM&apikey=demo) from Alpha Vantage.



```C++

std::string tsla_sector = tsla->Overview()->data["Sector"];
std::string tsla_employees = tsla->Overview()->data["FullTimeEmployees"];
std::cout << tsla_sector << '\n' << tsla_employees << '\n';

```
```
Output:
Consumer Cyclical
70757
```
---
**Historical Stock Data - Time Series**
The ```avapi::CompanyStock``` object contains the following methods for accessing historical stock data:

```C++

TimeSeries CompanyStock::getTimeSeries(const SeriesType &type, 
				       const bool &adjusted,
				       const std::string &interval = "30min");		


```

The ```avapi::TimeSeries``` class is comprised of an ```std::vector``` of  ```avapi::TimePair``` objects along with other important meta data. Each ```TimePair``` object simply contains an ```std::time_t``` UNIX timestamp and an ```std::vector<float>``` data vector. The data vector is ordered according to the ```TimeSeries``` type:

* Adjusted/Non-Adjusted **Intraday** data:
	* ```[open, high, low, close, volume]```
* Non-adjusted **daily**, **weekly**, and **monthly** data:
	* ```[open, high, low, close, volume]```
* Adjusted **daily** data:
	* ```[open, high, low, close, adjusted_close, volume, dividend, split_coefficient]```
* Adjusted **Weekly** and **Monthly** data:
	* ```[open, high, low, close, adjusted_close, volume, dividend]```
---
**Historical Stock Data - Intraday Time Series**
```C++

// Get an adjusted, intraday time series on a 15 minute interval
auto tsla_intraday = tsla.getTimeSeries(avapi::SeriesType::INTRADAY, 
					true, "15min");				
// Print the last 10 "open" values
for (size_t i = 0; i < 10; ++i)
	std::cout << tsla_intraday[i][0] <<  ' ';
std::cout << std::endl;

//Print last 3 rows
tsla_intraday.printData(3);

```
```
Output:
596.97 596.9 597 597.5 596.08 596.18 597.26 596.12 596 595.49
```
```
Output :
-------------------------------------------------------------------------------------------
|                      TSLA: TIME_SERIES_INTRADAY (15min, Adjusted)                       |
-------------------------------------------------------------------------------------------
|     timestamp|          open|          high|           low|         close|        volume|
-------------------------------------------------------------------------------------------
|    1615428000|        666.00|        667.95|        664.00|        664.56|      36428.00|
|    1615427100|        667.01|        667.47|        665.31|        665.65|      20616.00|
|    1615426200|        668.06|        668.50|        666.20|        667.00|      17452.00|
```
---
**Historical Stock Data - Daily, Weekly, and Monthly Time Series**

```C++

// Get non-adjusted daily, weekly, and monthly data
auto  daily = tsla->Stock()->getTimeSeries(avapi::SeriesType::DAILY, false);
auto  weekly = tsla->Stock()->getTimeSeries(avapi::SeriesType::WEEKLY, false);
auto  monthly = tsla->Stock()->getTimeSeries(avapi::SeriesType::MONTHLY, false);

// Get adjusted daily and weekly data
auto  daily_adj = tsla->Stock()->getTimeSeries(avapi::SeriesType::DAILY, true);
auto  weekly_adj = tsla->Stock()->getTimeSeries(avapi::SeriesType::WEEKLY, true);

// Print last 3 rows of each time series
daily.printData(3);
weekly.printData(3);
monthly.printData(3);
daily_adj.printData(3);
weekly_adj.printData(3);;

```
```
Output:
-------------------------------------------------------------------------------------------
|                         TSLA: TIME_SERIES_DAILY (Non-Adjusted)                          |
-------------------------------------------------------------------------------------------
|     timestamp|          open|          high|           low|         close|        volume|
-------------------------------------------------------------------------------------------
|    1615356000|        700.30|        717.85|        655.06|        668.06|   60605672.00|
|    1615269600|        608.18|        678.09|        595.21|        673.58|   67523328.00|
|    1615183200|        600.55|        620.12|        558.79|        563.00|   51786960.00|
```
```
Output:
-------------------------------------------------------------------------------------------
|                         TSLA: TIME_SERIES_WEEKLY (Non-Adjusted)                         |
-------------------------------------------------------------------------------------------
|     timestamp|          open|          high|           low|         close|        volume|
-------------------------------------------------------------------------------------------
|    1615356000|        600.55|        717.85|        558.79|        668.06|  179915952.00|
|    1614924000|        690.11|        721.11|        539.49|        597.95|  235272720.00|
|    1614319200|        762.64|        768.50|        619.00|        675.50|  217862432.00|
```
```
Output:
-------------------------------------------------------------------------------------------
|                        TSLA: TIME_SERIES_MONTHLY (Non-Adjusted)                         |
-------------------------------------------------------------------------------------------
|     timestamp|          open|          high|           low|         close|        volume|
-------------------------------------------------------------------------------------------
|    1615356000|        690.11|        721.11|        539.49|        668.06|  415188672.00|
|    1614319200|        814.29|        880.50|        619.00|        675.50|  518874400.00|
|    1611900000|        719.46|        900.40|        717.19|        793.53|  705739200.00|
```
```
Output:
----------------------------------------------------------------------------------------------------------------------------------------
|                                                  TSLA: TIME_SERIES_DAILY (Adjusted)                                                  |
----------------------------------------------------------------------------------------------------------------------------------------
|     timestamp|          open|          high|           low|         close|     adj_close|        volume|     dividends|   split_coeff|
----------------------------------------------------------------------------------------------------------------------------------------
|    1615356000|        700.30|        717.85|        655.06|        668.06|        668.06|   60605672.00|          0.00|          1.00|
|    1615269600|        608.18|        678.09|        595.21|        673.58|        673.58|   67523328.00|          0.00|          1.00|
|    1615183200|        600.55|        620.12|        558.79|        563.00|        563.00|   51786960.00|          0.00|          1.00|
```
```
Output:
-------------------------------------------------------------------------------------------------------------------------
|                                          TSLA: TIME_SERIES_WEEKLY (Adjusted)                                          |
-------------------------------------------------------------------------------------------------------------------------
|     timestamp|          open|          high|           low|         close|     adj_close|        volume|     dividends|
-------------------------------------------------------------------------------------------------------------------------
|    1615356000|        600.55|        717.85|        558.79|        668.06|        668.06|  179915952.00|          0.00|
|    1614924000|        690.11|        721.11|        539.49|        597.95|        597.95|  235272720.00|          0.00|
|    1614319200|        762.64|        768.50|        619.00|        675.50|        675.50|  217862432.00|          0.00|
```
---
**Historical Stock Data - Global Quote**

The ```avapi::CompanyStock::GlobalQuote``` class contains the stock of interest's current global quote data. It is constructed with the stock of interest's symbol, a UNIX timestamp, and a data vector ordered: ```[open, high, low, price, volume, previous_close, change, change%]```


```C++

auto tsla_quote = tsla->Stock()->getGlobalQuote();
tsla_quote.printData();

```
```
Output:
-----------------------------
|     TSLA Global Quote     |
-----------------------------
|Timestamp:       1615442400|
|Open:                699.40|
|High:                702.50|
|Low:                 677.18|
|Close:               699.60|
|Volume:         35858936.00|
|Prev_Close:          668.06|
|Change:               31.54|
|Change%:               4.72|
```
---
## Cryptocurrency Historical Pricing Data

Avapi also provides a simple interface for accessing historical pricing data for a cryptocurrency of interest. For the following examples, we will explore the available functions of the `avapi::Crypto` class with Bitcoin (“BTC”). With the `symbol` “BTC” and our Alpha Vantage API `key`, lets create a new `Crypto` object. Note: The `key` can be saved to a text file and then read with a provided helper function.

```C++

std::string symbol = "BTC";
std::string key = avapi::readFirstLineFromFile("api.key");
avapi::Crypto *btc = new avapi::Crypto(symbol, key);

```
---
**Historical Pricing Data - Daily, Weekly, and Monthly Time Series**


```C++

// Get daily, weekly, and monthly time series for "BTC" with the market "USD"
auto daily = btc->getTimeSeries(avapi::SeriesType::DAILY,"USD");
auto weekly = btc->getTimeSeries(avapi::SeriesType::WEEKLY,"USD");
auto monthly = btc->getTimeSeries(avapi::SeriesType::MONTHLY,"USD");

// Print last 3 rows
daily.printData(3);
weekly.printData(3);
monthly.printData(3);

```
```
Output:
-------------------------------------------------------------------------------------------
|                               BTC: DIGITAL_CURRENCY_DAILY                               |
-------------------------------------------------------------------------------------------
|     timestamp|    open (USD)|    high (USD)|     low (USD)|   close (USD)|        volume|
-------------------------------------------------------------------------------------------
|    1615442400|      55851.59|      56499.98|      55505.00|      56290.03|       2223.14|
|    1615356000|      54874.67|      57387.69|      53005.00|      55851.59|      84749.24|
|    1615269600|      52375.18|      54895.00|      51789.41|      54884.50|      71656.73|
```
```
Output:
-------------------------------------------------------------------------------------------
|                              BTC: DIGITAL_CURRENCY_WEEKLY                               |
-------------------------------------------------------------------------------------------
|     timestamp|    open (USD)|    high (USD)|     low (USD)|   close (USD)|        volume|
-------------------------------------------------------------------------------------------
|    1615442400|      50959.11|      57387.69|      49274.67|      56290.03|     225616.47|
|    1615096800|      45134.11|      52640.00|      44950.53|      50971.75|     490819.56|
|    1614492000|      57412.35|      57508.47|      43000.00|      45135.66|     737125.75|
```
```
Output:
-------------------------------------------------------------------------------------------
|                              BTC: DIGITAL_CURRENCY_MONTHLY                              |
-------------------------------------------------------------------------------------------
|     timestamp|    open (USD)|    high (USD)|     low (USD)|   close (USD)|        volume|
-------------------------------------------------------------------------------------------
|    1615442400|      45134.11|      57387.69|      44950.53|      56290.03|     716436.06|
|    1614492000|      33092.97|      58352.80|      32296.16|      45135.66|    2518242.25|
|    1612072800|      28923.63|      41950.00|      28130.00|      33092.98|    3435639.25|

```
---
**Pricing Data - Current Exchange Rate**


The ```avapi::ExchangeRate``` object is a class containing the stock of interest's current exchange rate for a specified market. It is constructed with a "from" and "to" symbol, a UNIX timestamp, and a data vector ordered: ```[Exchange Rate, Bid Price, Ask Price]```

```C++

auto exchange_rate = btc->Exchange("USD");
exchange_rate->printData();

```
```
Output:
------------------------------
|       Exchange Rate        |
|         BTC -> USD         |
------------------------------
|Timestamp:        1615521421|
|Exchange Rate:      57624.96|
|Bid Price:          57624.95|
|Ask Price:          57624.96|

```

## Parsing an Alpha Vantage time series csv file

```C++

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

auto series = avapi::parseCsvFile("daily_GME.csv",false);
series.title = "GME: Daily Time Series (Non Adjusted)";
series.printData(3);
    
```
```
Output:
-------------------------------------------------------------------------------------------
|                          GME: Daily Time Series (Non Adjusted)                          |
-------------------------------------------------------------------------------------------
|     timestamp|          open|          high|           low|         close|        volume|
-------------------------------------------------------------------------------------------
|    1613628000|         48.49|         48.87|         40.65|         40.69|   23990556.00|
|    1613541600|         49.77|         51.19|         44.56|         45.94|    9147635.00|
|    1613455200|         52.66|         53.50|         49.04|         49.51|    8175030.00|
```
