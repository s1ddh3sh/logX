#include "src/logger.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include "src/hooks/file_hook.h"
#include "src/hooks/console_hook.h"
#include "src/config/config.h"

int main()
{
    Config cfg = loadConfig("logger.conf");
    Logger logger(cfg.file.empty() ? "app.log" : cfg.file, cfg.queue_size);
    logger.setLogLvl(Logger::stringToLvl(cfg.level));

    if (!cfg.file.empty())
        logger.addHook(new FileHook(cfg.file));
    if (cfg.console)
        logger.addHook(new ConsoleHook());

    for (int i = 0; i < 50; i++)
    {
        logger.log(LogLevel::INFO, "Hello" + std::to_string(i));
    }
    logger.log(LogLevel::ERROR, "Quit");
    // sleep(1);

    return 0;
}