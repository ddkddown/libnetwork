#pragma once
#include <iostream>
#include <fstream>

extern "C" {
#include <assert.h>
}

class Logger;

#define LOG_DEBUG Logger::RecordLog()<<std::endl<<"[DEBUG]:"<<__FILE__<<":" \
                                    <<__func__<<":"<<__LINE__<<": "
#define LOG_INFO Logger::RecordLog()<<std::endl<<"[INFO]:"<<__FILE__<<":" \
                                    <<__func__<<":"<<__LINE__<<": "
#define LOG_WARN Logger::RecordLog()<<std::endl<<"[WARN]:"<<__FILE__<<":" \
                                    <<__func__<<":"<<__LINE__<<": "
#define LOG_ERR Logger::RecordLog()<<std::endl<<"[ERROR]:"<<__FILE__<<":" \
                                    <<__func__<<":"<<__LINE__<<": "
#define LOG_FATAL Logger::RecordLog()<<std::endl<<"[FATAL]:"<<__FILE__<<":" \
                                    <<__func__<<":"<<__LINE__<<": "

class Logger{
private:
    Logger(){}
    ~Logger(){}
public:
    static void InitLog(const char *file);

    static inline std::fstream& RecordLog() {
        return logStream;
    }

private:
    static std::fstream logStream;
};
