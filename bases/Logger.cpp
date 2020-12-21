#include "Logger.h"
std::fstream Logger::logStream_;

void Logger::InitLog(const char *file) {
    assert(NULL != file);
    logStream_.open(file,std::ios::app|std::ios::out);
}