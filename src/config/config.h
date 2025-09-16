#pragma once
#include <string>
struct Config
{
    std::string level = "INFO";
    size_t queue_size = 1024;
    std::string file;
    bool console = false;
};

Config loadConfig(const std::string &path);