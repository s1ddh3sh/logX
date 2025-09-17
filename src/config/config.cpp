#include "logX/config/config.h"
#include <fstream>

Config loadConfig(const std::string &path)
{
    Config cfg;
    std::ifstream in(path);
    std::string line;

    while (std::getline(in, line))
    {
        auto comment = line.find('#');
        if (comment != std::string::npos)
            line = line.substr(0, comment);

        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty())
            continue;
        auto eq = line.find('=');
        if (eq == std::string::npos)
            continue;

        std::string key = line.substr(0, eq);
        std::string value = line.substr(eq + 1);

        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        if (key == "level")
            cfg.level = value;
        else if (key == "queue_size")
            cfg.queue_size = std::stoul(value);
        else if (key == "file")
            cfg.file = value;
        else if (key == "console")
            cfg.console = (value == "true" || value == "1");
    }
    return cfg;
}