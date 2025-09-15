#ifndef FILE_HOOK_H
#define FILE_HOOK_H

#include "log_hook.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>

class FileHook : public LogHook
{
    int fd;

public:
    FileHook(const std::string &filename)
    {
        fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd == -1)
            throw std::runtime_error("Failed to open file");
    }
    ~FileHook() override
    {
        if (fd != -1)
            close(fd);
    }
    void write(const char *buf, size_t len) override
    {
        size_t total = 0;
        while (total < len)
        {
            ssize_t written;
            if ((written = ::write(fd, buf + total, len - total)) == -1)
            {
                perror("write");
                return;
            }
            total += written;
        }
    }
};

#endif