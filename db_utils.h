/* 
 * File:   DbUtils.h
 * Author: mac
 *
 * Created on August 31, 2011, 7:47 PM
 */

#ifndef __DBUTILS_H__
#define __DBUTILS_H__

#include <string>

#include <boost/program_options.hpp>
#include <boost/date_time.hpp>

#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

#include "config.h"

class DbUtils {
public:
    DbUtils(boost::program_options::variables_map &_configMap);
    virtual ~DbUtils();
    
    bool checkToken(std::string token);
    void deleteToken(std::string token);
    void cleanupSession(int now);
    
private:
    boost::program_options::variables_map configMap;
    sql::Driver *driver;
    sql::Connection *conn;
};

#endif	/* DBUTILS_H */

