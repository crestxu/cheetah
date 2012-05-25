#ifndef __PUB_SUB_MANAGER_H__
#define __PUB_SUB_MANAGER_H__

#include <string>
#include <set>

#include <boost/asio.hpp>

#include "server_connection.h"

const std::string CMD_CAMERA_START = "0";
const std::string CMD_CAMERA_STOP = "1";

class PubSubManager
{   
public:
    PubSubManager();

    void imgPublish(std::string sessionToken, http::RequestBody &reqBody);
    void imgSubscribe(std::string sessionToken, http::ServerConnectionPtr connPtr);
    void imgUnsubscribe(std::string sessionToken,
        http::ServerConnectionPtr connPtr);
    void cmdPublish(std::string sessionToken, std::string cmd);
    void cmdSubscribe(std::string sessionToken,
                        http::ServerConnectionPtr connPtr);
    void cmdUnsubscribe(std::string sessionToken);

private:
    void handleWrite(const boost::system::error_code &error,
            size_t bytesTransferred,
            boost::shared_ptr<boost::array<char, BUF_SIZE> > bufPtr);
    
    void handleImgWrite(const boost::system::error_code &error,
        size_t bytesTransferred,
        std::vector<boost::shared_ptr<boost::array<char, BUF_SIZE> > > bufPtrVec);

    typedef std::multimap<std::string, http::ServerConnectionPtr> ConnMap;
    
    ConnMap imgSubConnMap;
    ConnMap cmdSubConnMap;
};

#endif