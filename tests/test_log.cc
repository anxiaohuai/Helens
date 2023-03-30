#include <iostream>
#include "helens/log.h"
#include "helens/util.h"

int main(int argc, char** argv) {
    helens::Logger::ptr logger(new helens::Logger);
    logger->addAppender(helens::LogAppender::ptr(new helens::StdoutLogAppender));

    helens::FileLogAppender::ptr file_appender(new helens::FileLogAppender("./log.txt"));
    helens::LogFormatter::ptr fmt(new helens::LogFormatter("%d%T%p%T%m%n"));
    file_appender->setFormatter(fmt);
    file_appender->setLevel(helens::LogLevel::ERROR);

    logger->addAppender(file_appender);

    //helens::LogEvent::ptr event(new helens::LogEvent(__FILE__, __LINE__, 0, helens::GetThreadId(), helens::GetFiberId(), time(0)));
    //event->getSS() << "hello helens log";
    //logger->log(helens::LogLevel::DEBUG, event);
    std::cout << "hello helens log" << std::endl;

    HELENS_LOG_INFO(logger) << "test macro";
    HELENS_LOG_ERROR(logger) << "test macro error";

    HELENS_LOG_FMT_ERROR(logger, "test macro fmt error %s", "aa");

    auto l = helens::LoggerMgr::GetInstance()->getLogger("xx");
    HELENS_LOG_INFO(l) << "xxx";
    return 0;
}
