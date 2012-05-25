/* 
 * File:   TokenMonitor.h
 * Author: Phuoc Do
 *
 * Created on August 31, 2011, 4:50 PM
 */

#ifndef __TOKEN_MONITOR_H__
#define	__TOKEN_MONITOR_H__

#include <set>
#include <map>
#include <string>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "db_utils.h"

#define DISCONN_WAIT_TIME 10 /* in seconds */
#define CLEANUP_WAIT_TIME 300 /* 5 minutes */

/*
 * Token monitor runs periodically to check if session is no longer active
 * and needs cleanup.
 * It stores a map of token and the timestamp of last disconnect.
 * If client does not reconnect after DISCONNECT_WAIT_TIME, we remove its
 * session token from db.
 */

using namespace boost;

class TokenMonitor {
public:
    TokenMonitor(asio::io_service &ioService, DbUtils &_dbUtils);
    virtual ~TokenMonitor();
    
    void start();
    void stop();
    
    void track(std::string token, int disconnTime);
    void untrack(std::string token);
    
private:
    void disconnTimerFunction(const boost::system::error_code &error);
    void cleanupTimerFunction(const boost::system::error_code &error);
    
    // token and last disconnect timestamp
    std::map<std::string, int> tokenMap;
    asio::io_service &ioService;
    asio::deadline_timer disconnTimer;
    asio::deadline_timer cleanupTimer;
    DbUtils &dbUtils;
};

#endif	/* TOKENMONITOR_H */
