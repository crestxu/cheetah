#ifndef __CONNECTION_MANAGER_H__
#define __CONNECTION_MANAGER_H__

#include <boost/asio.hpp>
#include <set>

#include "connection.h"

using namespace boost;

namespace http
{

class ConnectionManager
{
public:
    ConnectionManager(asio::io_service &_ioService);
    void start(ConnectionPtr connPtr);
    void stop(ConnectionPtr connPtr);
    void stopAll();

private:
    asio::io_service &ioService;
    std::set<ConnectionPtr> connSet;    
};

}

#endif