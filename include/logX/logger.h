#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <atomic>
#include <memory>

#include "../../src/spsc.h"
#include "hooks/log_hook.h"
#include "config/config.h"
#include "hooks/file_hook.h"
#include "hooks/console_hook.h"

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
    Logger() : Logger("app.log") {}
    Logger(const std::string &filename);
    ~Logger();
    /**
     * @brief Queues a log message to be written to the file.
     * @param msg The message to log.
     */
    void log(LogLevel lvl, const std::string &msg);

#define DEFINE_LOG_FN(level) \
    void level(const std::string &msg) { log(LogLevel::level, msg); }

    DEFINE_LOG_FN(DEBUG)
    DEFINE_LOG_FN(INFO)
    DEFINE_LOG_FN(WARN)
    DEFINE_LOG_FN(ERROR)

#undef DEFINE_LOG_FN

    void addHook(LogHook *hook);
    void setLogLvl(LogLevel lvl);
    static const char *lvlToString(LogLevel lvl);
    static LogLevel stringToLvl(const std::string &lvl);

private:
    int fd;
    std::unique_ptr<SPSCQueue<std::string>> queue;
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
    void loadConfig(const std::string &configPath);
    void initHooks();
    std::string configFile;
    Config cfg;
};

#endif