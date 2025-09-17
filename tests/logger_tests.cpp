#include <logX/logger.h>
#include "logX/hooks/file_hook.h"
#include "logX/hooks/console_hook.h"
#include "logX/config/config.h"
#include <gtest/gtest.h>

#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

static std::string readFile(const std::string &path)
{
    std::ifstream ifs(path);
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    return buffer.str();
}

TEST(ConfigTests, LoadsConfigCorrectly)
{
    std::string confFile = "test_logger.conf";
    std::ofstream ofs(confFile);
    ofs << "[logger]\n";
    ofs << "level = WARN\n";
    ofs << "queue_size = 64\n";
    ofs << "\n[hooks]\n";
    ofs << "file = conf_test.log\n";
    ofs << "console = false\n";
    ofs.close();

    Config cfg = loadConfig(confFile);
    EXPECT_EQ(cfg.level, "WARN");
    EXPECT_EQ(cfg.queue_size, 64);
    EXPECT_EQ(cfg.file, "conf_test.log");
    EXPECT_FALSE(cfg.console);
}

TEST(LoggerTests, LogsToFile)
{
    std::string logfile = "app.log";
    if (fs::exists(logfile))
        fs::remove(logfile);

    Logger logger;

    logger.INFO("Hello test");
    logger.ERROR("Something failed");

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ASSERT_TRUE(fs::exists(logfile));

    std::string contents = readFile(logfile);

    EXPECT_NE(contents.find("Hello test"), std::string::npos);
    EXPECT_NE(contents.find("Something failed"), std::string::npos);
}

TEST(LoggerTests, RespectsLogLevel)
{
    std::string logfile = "app.log";
    if (fs::exists(logfile))
        fs::remove(logfile);

    Logger logger;
    logger.setLogLvl(LogLevel::ERROR);

    logger.DEBUG("debug msg");
    logger.INFO("info msg");
    logger.WARN("warn msg");
    logger.ERROR("error msg");

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::string content = readFile(logfile);
    EXPECT_EQ(content.find("debug msg"), std::string::npos);
    EXPECT_EQ(content.find("info msg"), std::string::npos);
    EXPECT_EQ(content.find("warn msg"), std::string::npos);
    EXPECT_NE(content.find("error msg"), std::string::npos);
}

TEST(LoggerTests, MultipleHooks)
{
    std::string logfile = "app.log";
    if (fs::exists(logfile))
        fs::remove(logfile);

    Logger logger;
    testing::internal::CaptureStdout();
    logger.INFO("Multi hook test");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::string consoleOut = testing::internal::GetCapturedStdout();

    std::string content = readFile(logfile);

    EXPECT_NE(consoleOut.find("Multi hook test"), std::string::npos);
    EXPECT_NE(content.find("Multi hook test"), std::string::npos);
}