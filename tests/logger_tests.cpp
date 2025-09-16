#include "logger.h"
#include "hooks/file_hook.h"
#include "hooks/console_hook.h"
#include <gtest/gtest.h>

#include <fstream>
#include <string>
#include <filesystem>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

TEST(LoggerTests, LogsToFile)
{
    std::string logfile = "test.log";
    if (fs::exists(logfile))
        fs::remove(logfile);

    Logger logger(logfile, 128);
    logger.addHook(new FileHook(logfile));
    logger.setLogLvl(LogLevel::DEBUG);

    logger.INFO("Hello test");
    logger.ERROR("Something failed");

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ASSERT_TRUE(fs::exists(logfile));

    std::ifstream in(logfile);
    std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    EXPECT_NE(contents.find("Hello test"), std::string::npos);
    EXPECT_NE(contents.find("Something failed"), std::string::npos);
}

TEST(LoggerTests, RespectsLogLevel)
{
    std::string logfile = "leveltest.log";
    if (fs::exists(logfile))
        fs::remove(logfile);

    Logger logger(logfile, 128);
    logger.addHook(new FileHook(logfile));
    logger.setLogLvl(LogLevel::ERROR);

    logger.INFO("This should not appear");
    logger.ERROR("This should appear");

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::ifstream in(logfile);
    std::string contents((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());

    EXPECT_EQ(contents.find("This should not appear"), std::string::npos);
    EXPECT_NE(contents.find("This should appear"), std::string::npos);
}