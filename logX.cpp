#include "src/logger.h"
#include <iostream>
#include <string>

Logger logger("app.log", 1024);
int main()
{
    for (int i = 0; i < 100; i++)
    {
        logger.log("Hello" + std::to_string(i));
    }
    return 0;
}