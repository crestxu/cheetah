/* 
 * File:   token_handler.h
 * Author: Phuoc Do
 *
 * Created on August 18, 2011, 9:58 PM
 */

#ifndef __TOKEN_HANDLER_H__
#define	__TOKEN_HANDLER_H__

#include <string>

#include <boost/program_options.hpp>

#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

#include "config.h"
#include "db_utils.h"

class TokenHandler
{
public:
    TokenHandler(DbUtils &_dbUtils);
    ~TokenHandler();
    
    bool check(std::string token);

private:
    DbUtils &dbUtils;
    // use void * to avoid include of mysql cppconn headers
    sql::Driver *driver;
    sql::Connection *conn;
};

#endif	/* __TOKEN_HANDLER_H__ */

