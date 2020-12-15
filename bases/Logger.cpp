#include "Logger.h"
std::fstream Logger::logStream;

void Logger::InitLog(const char *file) {
    assert(NULL != file);
    logStream.open(file,std::ios::app|std::ios::out);
}