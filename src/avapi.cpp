#include "../inc/avapi.h"
#include "../inc/csv-parser.h"

namespace avapi {

// Quote constructor
Quote::Quote(std::string symbol, std::string api_key)
    : m_symbol(symbol), m_api_key(api_key)
{
}

// Quote deconstructor
Quote::~Quote() {}

// Get Intraday stock data on a set interval.
std::vector<std::vector<float>> Quote::getIntraday(std::string interval)
{
    std::string url =
        "https://www.alphavantage.co/"
        "query?function=TIME_SERIES_INTRADAY&symbol={symbol}&interval="
        "{interval}&apikey={api}&datatype=csv";

    string_replace(url, "{symbol}", this->m_symbol);
    string_replace(url, "{interval}", interval);
    string_replace(url, "{api}", this->m_api_key);

    std::cout << url;

    std::string file_name =
        "..\\..\\data\\intraday_" + interval + "_" + this->m_symbol + ".csv";
    download(url, file_name);

    return parse(file_name);
}

// Get daily stock data from last_n days.
std::vector<std::vector<float>> Quote::getDaily(int last_n)
{
    std::string url =
        "https://www.alphavantage.co/"
        "query?function=TIME_SERIES_DAILY&symbol={symbol}&apikey={api}&"
        "datatype=csv";
    string_replace(url, "{symbol}", this->m_symbol);
    string_replace(url, "{api}", this->m_api_key);

    std::string file_name = "..\\..\\data\\daily_" + this->m_symbol + ".csv";
    download(url, file_name);
    return parse(file_name);
}

// Get weekly stock data from last_n weeks.
std::vector<std::vector<float>> Quote::getWeekly(int last_n)
{
    std::string url =
        "https://www.alphavantage.co/"
        "query?function=TIME_SERIES_WEEKLY&symbol={symbol}&apikey={api}&"
        "datatype=csv";

    string_replace(url, "{symbol}", this->m_symbol);
    string_replace(url, "{api}", this->m_api_key);

    std::string file_name = "..\\..\\data\\weekly_" + this->m_symbol + ".csv";
    download(url, file_name);
    return parse(file_name);
}

// Get monthly stock data from last_n months.
std::vector<std::vector<float>> Quote::getMonthly(int last_n)
{
    std::string url =
        "https://www.alphavantage.co/"
        "query?function=TIME_SERIES_MONTHLY&symbol={symbol}&apikey={api}&"
        "datatype=csv";

    string_replace(url, "{symbol}", this->m_symbol);
    string_replace(url, "{api}", this->m_api_key);

    std::string file_name = "..\\..\\data\\monthly_" + this->m_symbol + ".csv";
    download(url, file_name);
    return parse(file_name);
}

// Get latest global quote for stock
std::vector<std::vector<float>> Quote::getGlobalQuote()
{
    std::string url =
        "https://www.alphavantage.co/"
        "query?function=GLOBAL_QUOTE&symbol={symbol}&apikey={api}&datatype=csv";

    string_replace(url, "{symbol}", this->m_symbol);
    string_replace(url, "{api}", this->m_api_key);

    std::string file_name =
        "..\\..\\data\\global_quote_" + this->m_symbol + ".csv";
    download(url, file_name);
    return parse(file_name);
}

std::vector<std::time_t> Quote::getTimeCol() { return this->m_timeCol; }

// Curls stock data using "t_url" and saves it to .csv "file_name"
void Quote::download(const std::string &t_url, const std::string &file_name)
{
    const char *url = t_url.c_str();
    const char *file = file_name.c_str();
    CURL *curl;
    FILE *fp;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(file, "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);

        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(fp);
    }
}

std::vector<std::vector<float>> Quote::parse(const std::string &file_name)
{
    std::string time_str = "%Y-%m-%d";
    if (file_name.find("intraday")) {
        time_str = "%Y-%m-%d %H:%M:%S";
    }

    csv::CSVReader reader(file_name);

    std::tm t{};

    std::vector<float> x;
    float val;
    std::vector<std::vector<float>> xy;

    std::vector<std::time_t> time_vec;

    for (csv::CSVRow &row : reader) {
        std::vector<float> x;
        for (int i = 0; i < 6; ++i) {
            if (i == 0) {
                std::stringstream ss;
                ss << row[i].get<std::string>();
                ss >> std::get_time(&t, time_str.c_str());
                time_t time = mktime(&t);
                time_vec.push_back(time);
            }
            else {
                val = row[i].get<float>();

                x.push_back(val);
            }
        }
        xy.push_back(x);
    }

    this->m_timeCol = time_vec;

    return xy;
}

// Replaces string "from" to "to" within "str"
bool string_replace(std::string &str, const std::string from,
                    const std::string to)
{
    size_t start = str.find(from);
    if (start == std::string::npos) {
        return false;
    }
    str.replace(start, from.length(), to);
    return true;
}

// Reads the first line from provided file
// Used mainly for reading an api.key file with a single line
std::string readFirstLineFromFile(const std::string &file_name)
{
    std::string api_key;
    std::ifstream file(file_name);
    if (file.is_open()) {
        std::getline(file, api_key);
        file.close();
    }
    else {
        std::cout << "Unable to open file: " << file_name;
    }
    return api_key;
}

} // namespace avapi