#include "connection_manager.h"

using namespace http;

ConnectionManager::ConnectionManager(asio::io_service &_ioService) :
                    ioService(_ioService)
{
}

void ConnectionManager::start(ConnectionPtr connPtr)
{
    this->connSet.insert(connPtr);
    connPtr->start();
}

void ConnectionManager::stop(ConnectionPtr connPtr)
{
    connPtr->stop();
    this->connSet.erase(connPtr);
}

void ConnectionManager::stopAll()
{
    std::set<ConnectionPtr>::const_iterator iter;
    for (iter = this->connSet.begin(); iter != this->connSet.end(); iter++)
    {
        (*iter)->stop();
    }
    this->connSet.clear();
}
