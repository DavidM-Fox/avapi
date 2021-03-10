#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>
#include "avapi/misc.hpp"

namespace avapi {
/// @brief   Replaces a substring within a given string
/// @param   str The given string
/// @param   from The substring being replaced by "to"
/// @param   to The substring replacing "from"
/// @returns A success/fail bool
bool stringReplace(std::string &str, const std::string &from,
                   const std::string &to)
{
    size_t start = str.find(from);
    if (start == std::string::npos) {
        return false;
    }
    str.replace(start, from.length(), to);
    return true;
}

/// @brief   Reads the first line from a given file
/// @param   file_path File to be read's path
/// @returns first line as an std::string
std::string readFirstLineFromFile(const std::string &file_path)
{
    std::string api_key = "";
    std::ifstream file(file_path.c_str());

    if (file.is_open()) {
        std::getline(file, api_key);
        file.close();
    }
    else {

        std::exception ex(
            std::string("\"" + file_path + "\" cannot be opened").c_str());
        throw ex;
    }
    return api_key;
}

/// @brief   Converts date + time string "%Y-%m-%d %H:%M:%S" to Unix Timestamp
/// (seconds since unic epoch)
/// @param   input The input string to be converted
/// @returns The time as an std::time_t unix timestamp
std::time_t toUnixTimestamp(const std::string &input)
{
    std::tm t{};
    std::istringstream ss(input);
    std::string format = "%Y-%m-%d %H:%M:%S";
    ss >> std::get_time(&t, format.c_str());
    return mktime(&t);
}

/// @brief   Test if a string is in JSON format
/// @param   data The string to be tested
/// @returns bool
bool isJsonString(const std::string &data)
{
    try {
        nlohmann::json parser = nlohmann::json::parse(data);
    }
    catch (...) {
        return false;
    }
    return true;
}

} // namespace avapi