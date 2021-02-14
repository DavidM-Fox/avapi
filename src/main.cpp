//#include "../inc/csv-parser.h"
#include "../inc/avapi.h"
//#include "../inc/csv-parser.h"
#include "../inc/rapidcsv.h"
#include <vector>

int main()
{
    // std::string api_key = avapi::readFirstLineFromFile("../../api.key");
    // avapi::Quote quote("TSLA", api_key);
    // auto xy = quote.getIntraday("15min");
    // auto time = quote.getTimeCol();
    //
    // for (auto &val : time) {
    // std::cout << val << '\n';
    // }
    // std::cout << std::endl;
    rapidcsv::Document doc("../../data/intraday_15min_TSLA.csv");
    std::vector<float> col = doc.GetColumn<float>("open");

    for (auto &i : col) {
        std::cout << i << '\n';
    }
    std::cout << std::endl;
}