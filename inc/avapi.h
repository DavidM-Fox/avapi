#ifndef AVAPI_H
#define AVAPI_H

#include <curl/curl.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <string>

class quote {
public:
    quote(std::string t_symbol);
    ~quote();

    // Setters
    void set_function(std::string t_function);
    void set_symbol(std::string t_symbol);
    void set_interval(std::string t_interval);

    // Getters
    std::string get_function();
    std::string get_symbol();
    std::string get_interval();

    void download(const std::string &t_function);

private:
    bool build_url(const std::string &t_function);
    bool string_replace(std::string &str, const std::string from,
                        const std::string to);

    std::string m_api_key;
    std::string m_symbol;
    std::string m_function;
    std::string m_interval;
    std::string m_fileName;
    std::string m_url;
};

#endif // AVAPI_H