#include "src/logger.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include "src/hooks/file_hook.h"
#include "src/hooks/console_hook.h"

Logger logger("app.log", 1024);
int main()
{
    logger.addHook(new FileHook("app.log"));
    logger.addHook(new ConsoleHook);
    for (int i = 0; i < 50; i++)
    {
        logger.log(LogLevel::INFO, "Hello" + std::to_string(i));
    }
    logger.log(LogLevel::ERROR, "Quit");
    // sleep(1);

    return 0;
}