#ifndef LOG_HOOK_H
#define LOG_HOOK_H

#include <cstddef>

class LogHook
{
public:
    virtual ~LogHook() = default;
    virtual void write(const char *buf, size_t len) = 0;
};

#endif