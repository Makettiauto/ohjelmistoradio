// Logger.cpp
#include "Logger.hpp"
#include <iostream>
#include <mutex>
#include <set>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>

/**************************************************************
	HOW TO USE!!

    You can call the logger with
    Logger::getLogger()->info("What u want{}", then the variable);
**************************************************************/


std::shared_ptr<spdlog::logger> Logger::fileLogger = nullptr;
std::set<std::string> Logger::loggedMessages;
std::mutex Logger::messageMutex;


void Logger::initialize() {
    if (!fileLogger) {
        try {
            // Create a thread pool with 2 background logging thread and a queue with 16384 items
            static auto thread_pool = std::make_shared<spdlog::details::thread_pool>(16384, 2);
            
            // Async file sink
            auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("transmission.log", true);
            fileLogger = std::make_shared<spdlog::async_logger>(
                "async_file_logger",
                file_sink,
                thread_pool,
                spdlog::async_overflow_policy::block);

            // Set as default logger
            spdlog::set_default_logger(fileLogger);
            spdlog::set_level(spdlog::level::info); // Or debug/trace for more verbosity
            spdlog::set_pattern("[%H:%M:%S %z] [%^%l%$] %v");
        } catch (const spdlog::spdlog_ex& ex) {
            std::cerr << "Async log initialization failed: " << ex.what() << std::endl;
        }
    }
}

std::shared_ptr<spdlog::logger> Logger::getLogger() {
    if (!fileLogger) {
        initialize();
    }
    return fileLogger;
}

void Logger::infoOnce(const std::string& message) {
    std::lock_guard<std::mutex> lock(messageMutex);
    if (loggedMessages.find(message) == loggedMessages.end()) {
        getLogger()->info(message);
        loggedMessages.insert(message);
    }
}
