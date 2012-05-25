/* 
 * File:   TokenMonitor.cpp
 * Author: Phuoc Do
 * 
 * Created on August 31, 2011, 4:50 PM
 */

#include "token_monitor.h"

TokenMonitor::TokenMonitor(asio::io_service &_ioService, DbUtils &_dbUtils) :
        ioService(_ioService),
        disconnTimer(_ioService),
        cleanupTimer(_ioService),
        dbUtils(_dbUtils) {
}

TokenMonitor::~TokenMonitor() {
}

void TokenMonitor::start() {
    this->disconnTimer.expires_from_now(posix_time::seconds(DISCONN_WAIT_TIME));
    this->disconnTimer.async_wait(boost::bind(
            &TokenMonitor::disconnTimerFunction, this, _1));
    
    this->cleanupTimer.expires_from_now(posix_time::seconds(CLEANUP_WAIT_TIME));
    this->cleanupTimer.async_wait(boost::bind(
            &TokenMonitor::cleanupTimerFunction, this, _1));
}

void TokenMonitor::stop() {
    this->disconnTimer.cancel();
    this->cleanupTimer.cancel();
}

void TokenMonitor::track(std::string token, int disconnTime) {
    this->tokenMap[token] = disconnTime;
}

void TokenMonitor::untrack(std::string token) {
    this->tokenMap.erase(token);
}

void TokenMonitor::disconnTimerFunction(const boost::system::error_code &error) {
    if (!error) {
        // get current time
        posix_time::ptime epoch(gregorian::date(1970, 1, 1));
        posix_time::ptime now = posix_time::second_clock::local_time();
        posix_time::time_duration::sec_type unixTime =
                (now - epoch).total_seconds();
        
        std::set<std::string> expiredTokens;
        
        // check token set
        std::map<std::string, int>::const_iterator mapIter;
        for (mapIter = this->tokenMap.begin(); mapIter != this->tokenMap.end();
                mapIter++) {
            std::pair<std::string, int> tokenPair = *mapIter;
            if (unixTime > tokenPair.second) {
                expiredTokens.insert(tokenPair.first);
            }
        }
        
        std::set<std::string>::const_iterator expIter;
        for (expIter = expiredTokens.begin(); expIter != expiredTokens.end();
                expIter++) {
            // delete from db
            this->dbUtils.deleteToken(*expIter);
            this->tokenMap.erase(*expIter);
        }
        
        this->disconnTimer.expires_from_now(posix_time::seconds(
                DISCONN_WAIT_TIME));
        this->disconnTimer.async_wait(boost::bind(
            &TokenMonitor::disconnTimerFunction, this, _1));
    }
}

void TokenMonitor::cleanupTimerFunction(
        const boost::system::error_code &error) {
    if (!error) {
        // get current time
        posix_time::ptime epoch(gregorian::date(1970, 1, 1));
        posix_time::ptime now = posix_time::second_clock::universal_time();
        posix_time::time_duration::sec_type unixTime =
                (now - epoch).total_seconds();
        
        this->dbUtils.cleanupSession(unixTime);
        
        this->cleanupTimer.expires_from_now(posix_time::seconds(
                CLEANUP_WAIT_TIME));
        this->cleanupTimer.async_wait(boost::bind(
            &TokenMonitor::cleanupTimerFunction, this, _1));
    }
}