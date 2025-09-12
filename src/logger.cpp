#include "logger.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <stdio.h>
#include <pthread.h>

Logger::Logger(const std::string &filename, size_t queue_size = 1024) : queue(queue_size), stop(false)
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
void Logger::log(const std::string &msg)
{
    while (!queue.enqueue(msg))
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
                write(fd, batch.data(), batch.size());
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
}