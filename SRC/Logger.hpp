#pragma once
#include <spdlog/spdlog.h>
#include <memory>
#include <mutex>
#include <string>
#include <set> 

class Logger {
public:
    static void initialize();
    static std::shared_ptr<spdlog::logger> getLogger();
    static void infoOnce(const std::string& message);

private:
    static std::shared_ptr<spdlog::logger> fileLogger;
    static std::set<std::string> loggedMessages; // now it works
    static std::mutex messageMutex;
};
