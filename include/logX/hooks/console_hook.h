#ifndef CONSOLE_HOOK_H
#define CONSOLE_HOOK_H

#include "log_hook.h"
#include <iostream>
class ConsoleHook : public LogHook
{
public:
    void write(const char *buf, size_t len) override
    {
        std::cout.write(buf, len);
        std::cout.flush();
    }
};

#endif