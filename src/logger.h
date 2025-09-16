#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <atomic>
#include "spsc.h"
#include "hooks/log_hook.h"

enum class LogLevel
{
    DEBUG,
    INFO,
    WARN,
    ERROR,
    NONE
};

/**
 * @class Logger
 * @brief Async Logger to write msgs to a file using worker thread
 */
class Logger
{
public:
    Logger(const std::string &filename, size_t queue_size);
    ~Logger();
    /**
     * @brief Queues a log message to be written to the file.
     * @param msg The message to log.
     */
    void log(LogLevel lvl, const std::string &msg);
    void addHook(LogHook *hook);
    void setLogLvl(LogLevel lvl);
    static const char *lvlToString(LogLevel lvl);
    static LogLevel stringToLvl(const std::string &lvl);

private:
    int fd;
    SPSCQueue<std::string> queue;
    pthread_t worker;
    std::atomic<bool> stop; // Flag to send stop signal to worker
    LogLevel logLvl;
    std::vector<LogHook *> hooks;

    /**
     * @brief Entry point for worker thread
     * @param arg Pointer to Logger instance
     * @return NULL
     */
    static void *worker_thread(void *arg);
    // Thread fn to process the log queue
    void run();
};

#endif