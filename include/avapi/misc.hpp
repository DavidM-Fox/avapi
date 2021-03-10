// Misc functions for avapi
#ifndef AVAPIMISC_H
#define AVAPIMISC_H
#include <string>
#include <iomanip>

namespace avapi {

bool stringReplace(std::string &str, const std::string &from,
                   const std::string &to);

std::string readFirstLineFromFile(const std::string &file_path);

std::time_t toUnixTimestamp(const std::string &input);
bool isJsonString(const std::string &data);

} // namespace avapi

#endif