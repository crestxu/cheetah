/* 
 * File:   DbUtils.cpp
 * Author: mac
 * 
 * Created on August 31, 2011, 7:47 PM
 */

#include "db_utils.h"

DbUtils::DbUtils(boost::program_options::variables_map &_configMap) :
        configMap(_configMap) {
    std::string host = this->configMap[CONFIG_DB_HOST].as<std::string>();
    std::string username =
        this->configMap[CONFIG_DB_USERNAME].as<std::string>();
    std::string password =
        this->configMap[CONFIG_DB_PASSWORD].as<std::string>();
    std::string dbName =
        this->configMap[CONFIG_DB_NAME].as<std::string>();
    this->driver = get_driver_instance();
    this->conn = driver->connect(
        host.c_str(), username.c_str(), password.c_str());
    this->conn->setSchema(dbName.c_str());
}

DbUtils::~DbUtils() {
    this->conn->close();
    delete this->conn;
}

bool DbUtils::checkToken(std::string token) {
    if (token == "")
        return false;
    
    bool ret = true;
    sql::ResultSet *res = NULL;
    sql::PreparedStatement *prepStmt = NULL;

    try
    {
        prepStmt = ((sql::Connection *)conn)->prepareStatement(
                        "SELECT * FROM active_sessions "
                        "WHERE session_token=?");
        prepStmt->setString(1, token.c_str());
        prepStmt->execute();

        res = prepStmt->getResultSet();

        if (res->rowsCount() != 1)
            ret = false;
        else
        {
            res->next();
            // check timestamp
            unsigned int expiredAt = res->getUInt("expired_at");
            boost::posix_time::ptime now(
                boost::posix_time::second_clock::universal_time());
            boost::posix_time::ptime startEpoch(
                boost::gregorian::date(1970, 1, 1));
            boost::posix_time::time_duration::sec_type nowUnixTime =
                    (now - startEpoch).total_seconds();
            if (nowUnixTime > (int)expiredAt)
                ret = false;
        }
    } catch (...)
    {
        ret = false;
    }

    if (res)
        delete res;
    if (prepStmt)
        delete prepStmt;

    return ret;
}

void DbUtils::deleteToken(std::string token) {
    sql::PreparedStatement *prepStmt = NULL;

    try
    {
        prepStmt = ((sql::Connection *)conn)->prepareStatement(
                        "DELETE FROM active_sessions "
                        "WHERE session_token=?");
        prepStmt->setString(1, token.c_str());
        prepStmt->execute();
    } catch (...)
    { }

    if (prepStmt)
        delete prepStmt;
}

void DbUtils::cleanupSession(int now) {
    sql::PreparedStatement *prepStmt = NULL;

    try
    {
        prepStmt = ((sql::Connection *)conn)->prepareStatement(
                        "DELETE FROM active_sessions "
                        "WHERE expired_at < ?");
        prepStmt->setInt(1, now);
        prepStmt->execute();
    } catch (...)
    { }

    if (prepStmt)
        delete prepStmt;
}