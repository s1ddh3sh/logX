#include "logger.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <stdio.h>
#include <pthread.h>

Logger::Logger(const std::string &filename, size_t queue_size) : queue(queue_size), stop(false), logLvl(LogLevel::INFO)
{
    fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        throw std::runtime_error("Failed to open file");
    }
    // starts worker
    pthread_create(&worker, NULL, &Logger::worker_thread, this);
}
Logger::~Logger()
{
    stop.store(true, std::memory_order_release);
    pthread_join(worker, NULL);
    if (fd != -1)
    {
        if (close(fd) == -1)
        {
            perror("close");
        }
    }
}
void Logger::log(LogLevel lvl, const std::string &msg)
{
    if (lvl < logLvl)
        return;
    std::string m = "[" + std::string(lvlToString(lvl)) + "] " + msg;
    while (!queue.enqueue(m))
    {
        sched_yield();
    }
}

void *Logger::worker_thread(void *arg)
{
    Logger *self = static_cast<Logger *>(arg);
    self->run();
    return NULL;
}
static void writeAll(int fd, const char *buf, size_t len)
{
    size_t total = 0;
    while (total < len)
    {
        ssize_t written;
        if ((written = write(fd, buf + total, len - total)) == -1)
        {
            perror("write");
            return;
        }
        total += written;
    }
}

void Logger::run()
{
    std::string msg;
    std::string batch;
    batch.reserve(4096);
    while (!stop.load())
    {
        while (queue.dequeue(msg))
        {
            batch.append(msg);
            batch.push_back('\n');

            if (batch.size() >= 4096)
            {
                if (write(this->fd, batch.data(), batch.size()) == -1)
                    perror("write");
                batch.clear();
            }
        }
        if (!batch.empty())
        {
            write(fd, batch.data(), batch.size());
            batch.clear();
        }
        sched_yield();
    }
    // final drain
    while (queue.dequeue(msg))
    {
        batch.append(msg);
        batch.push_back('\n');
        if (batch.size() >= 4096)
        {
            writeAll(this->fd, batch.data(), batch.size());
            batch.clear();
        }
    }
    if (!batch.empty())
    {
        writeAll(this->fd, batch.data(), batch.size());
    }
}

const char *Logger::lvlToString(LogLevel lvl)
{
    switch (lvl)
    {
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::WARN:
        return "WARN";
    case LogLevel::ERROR:
        return "ERROR";
    case LogLevel::NONE:
        return "NONE";
    default:
        return "UNKNOWN";
    }
}