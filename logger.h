#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <sstream>
#include <fstream>
#include <iomanip>
#include <time.h>

enum LogLevel { LL_CRITICAL, LL_ERROR, LL_WARNING, LL_INFO };
extern std::string LogLevelString[];

const LogLevel DEFAULT_LOGLEVEL = LL_ERROR;

using namespace std;

class Logger
{
public:
    Logger(string logName);
    ~Logger();

    void write(string log);
    void writeLn(string log);
    
    void write(LogLevel level, string log);
    void writeLn(LogLevel level, string log);
    
    void setLogLevel(LogLevel level);
    LogLevel getLogLevel();

    void close();

    static string getLogLevelString(enum LogLevel level);

private:
    string getCurrentTime();
    string fillZeros(string str, int len);
    string fillZeros(int num, int len);

    string logName;
    LogLevel logLevel;
    ofstream logStream;
};

#endif /* __LOGGER_H__ */
