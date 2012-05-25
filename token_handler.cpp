#include "token_handler.h"

#include <boost/date_time/posix_time/posix_time.hpp>

TokenHandler::TokenHandler(DbUtils &_dbUtils) :
    dbUtils(_dbUtils)
{
}

TokenHandler::~TokenHandler()
{
}

bool TokenHandler::check(std::string token)
{
    return dbUtils.checkToken(token);
}