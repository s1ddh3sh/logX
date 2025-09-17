#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <stdio.h>
#include <pthread.h>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>

#include "logX/logger.h"
#include "logX/hooks/log_hook.h"

#include <filesystem>
namespace fs = std::filesystem;

using namespace std::chrono;

std::string timestamp()
{
    auto now = system_clock::now();
    time_t t = system_clock::to_time_t(now);
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&t), "%F %T")
        << "." << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

Logger::Logger(const std::string &configPath = "logger.conf") : stop(false), logLvl(LogLevel::INFO)
{
    if (fs::exists(configPath))
    {
        cfg = ::loadConfig(configPath); // private member Config cfg;
    }
    else
    {
        // default config
        cfg.level = "INFO";
        cfg.queue_size = 1024;
        cfg.file = "app.log";
        cfg.console = true;
    }
    if (cfg.queue_size == 0)
        cfg.queue_size = 1024;

    queue = std::make_unique<SPSCQueue<std::string>>(cfg.queue_size);

    if (!cfg.file.empty())
        addHook(new FileHook(cfg.file));
    if (cfg.console)
        addHook(new ConsoleHook());

    logLvl = stringToLvl(cfg.level);

    // starts worker
    pthread_create(&worker, NULL, &Logger::worker_thread, this);
}
Logger::~Logger()
{
    stop.store(true, std::memory_order_release);
    pthread_join(worker, NULL);
    for (auto hook : hooks)
        delete hook;
}
void Logger::addHook(LogHook *hook)
{
    hooks.push_back(hook);
}

void Logger::log(LogLevel lvl, const std::string &msg)
{
    if (lvl < logLvl)
        return;
    std::string m = "[" + timestamp() + "]" + "[" + std::string(lvlToString(lvl)) + "] " + msg;
    while (!queue->enqueue(m))
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
        while (queue->dequeue(msg))
        {
            batch.append(msg);
            batch.push_back('\n');

            if (batch.size() >= 4096)
            {
                for (auto hook : hooks)
                {
                    hook->write(batch.data(), batch.size());
                }
                batch.clear();
            }
        }
        if (!batch.empty())
        {
            for (auto hook : hooks)
            {
                hook->write(batch.data(), batch.size());
            }
            batch.clear();
        }
        sched_yield();
    }
    // final drain
    while (queue->dequeue(msg))
    {
        batch.append(msg);
        batch.push_back('\n');
        if (batch.size() >= 4096)
        {
            for (auto hook : hooks)
                hook->write(batch.data(), batch.size());
            batch.clear();
        }
    }
    if (!batch.empty())
    {
        for (auto hook : hooks)
            hook->write(batch.data(), batch.size());
    }
}

void Logger::setLogLvl(LogLevel lvl)
{
    this->logLvl = lvl;
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

LogLevel Logger::stringToLvl(const std::string &s)
{
    if (s == "INFO")
        return LogLevel::INFO;
    if (s == "DEBUG")
        return LogLevel::DEBUG;
    if (s == "WARN")
        return LogLevel::WARN;
    if (s == "ERROR")
        return LogLevel::ERROR;
    return LogLevel::INFO;
}