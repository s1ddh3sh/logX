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
    if (cfg.queue_size == 0)
        cfg.queue_size = 1024;

    Logger logger(cfg.file.empty() ? "app.log" : cfg.file, cfg.queue_size);
    logger.setLogLvl(Logger::stringToLvl(cfg.level));

    if (!cfg.file.empty())
        logger.addHook(new FileHook(cfg.file));
    if (cfg.console)
        logger.addHook(new ConsoleHook());

    logger.INFO("Hello 1");

    logger.ERROR("Quit");
    // sleep(1);

    return 0;
}