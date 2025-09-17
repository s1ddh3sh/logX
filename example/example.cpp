#include <logX/logger.h>

int main()
{
    Logger logger("logger.conf");
    logger.INFO("Hello 1");
    logger.ERROR("Quit");

    return 0;
}