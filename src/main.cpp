#include "../inc/avapi.h"

int main()
{
    avapi::time_series series = avapi::parseCsvFile("../../ex_data/daily.csv");
    avapi::printSeries(series);
}