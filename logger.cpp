#include "logger.h"

std::string LogLevelString[] = { "CRITICAL", "ERROR", "WARNING", "INFO" };

Logger::Logger(string logName)
{
    this->logName = logName;
    this->logStream.open(this->logName.c_str(), ios_base::app);
    this->logLevel = DEFAULT_LOGLEVEL; 
}

Logger::~Logger()
{
    this->logStream.flush();
    this->logStream.close();
}

void Logger::write(string log)
{
    this->write(this->logLevel, log);
}

void Logger::writeLn(string log)
{
    this->writeLn(this->logLevel, log);
}

void Logger::write(LogLevel level, string log)
{
    if (level <= this->logLevel)
    {
        string timeStr = getCurrentTime();
        this->logStream << timeStr << " (" << Logger::getLogLevelString(level)
                        << "): " << log.c_str();
    }
}

void Logger::writeLn(LogLevel level, string log)
{
    if (level <= this->logLevel)
    {
        this->write(level, log);
        this->logStream << endl;        
    }
}

void Logger::close()
{
    this->logStream.close();
}

string Logger::getCurrentTime()
{
    ostringstream strStream;
    time_t now;
    time(&now);
    struct tm *current = localtime(&now);
    
    strStream << current->tm_year + 1900 << "-"
              << setw(2) << fillZeros(current->tm_mon + 1, 2) << "-"
              << setw(2) << fillZeros(current->tm_mday, 2) << " "
              << setw(2) << fillZeros(current->tm_hour, 2) << "."
              << setw(2) << fillZeros(current->tm_min, 2) << "."
              << setw(2) << fillZeros(current->tm_sec, 2);
    
    return strStream.str();
}

string Logger::fillZeros(string str, int len)
{
    string zeroStr = "";
    
    if (len > (int)str.length())
    {
        for (int i = 0; i < len - (int)str.length(); i++)
        {
            zeroStr += "0";   
        }
    }
    
    return zeroStr + str;
}

string Logger::fillZeros(int num, int len)
{
    ostringstream strStream;
    
    strStream << num;
    
    return fillZeros(strStream.str(), len);
}

void Logger::setLogLevel(LogLevel level)
{
    this->logLevel = level;
}

LogLevel Logger::getLogLevel()
{
    return this->logLevel;
}

string Logger::getLogLevelString(enum LogLevel level)
{
    return LogLevelString[level];
}
