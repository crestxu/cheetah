#include "pub_sub_manager.h"

PubSubManager::PubSubManager()
{
}

void PubSubManager::imgPublish(std::string sessionToken,
        http::RequestBody &reqBody)
{
    pair<ConnMap::iterator, ConnMap::iterator> connRange =
            this->imgSubConnMap.equal_range(sessionToken);
    for (ConnMap::iterator connIter = connRange.first;
            connIter != connRange.second; connIter++)
    {
        http::ServerConnectionPtr connPtr = connIter->second;
        
        std::vector<boost::shared_ptr<boost::array<char, BUF_SIZE> > > bufPtrVec;
        std::vector<asio::const_buffer> bufVec;
        
        std::stringstream ss;
        ss << std::hex << (reqBody.size);

        std::string chunkStr = ss.str();
        chunkStr += "\r\n";
        boost::shared_ptr<boost::array<char, BUF_SIZE> > headerPtr(
                new boost::array<char, BUF_SIZE>());
        memcpy(headerPtr->data(), chunkStr.c_str(), chunkStr.size());
        bufVec.push_back(asio::buffer(headerPtr->data(), chunkStr.size()));
        bufPtrVec.push_back(headerPtr);
        for (std::vector<http::BodyBufPair>::const_iterator bodyIter =
            reqBody.list.begin(); bodyIter != reqBody.list.end(); bodyIter++)
        {
            boost::shared_ptr<boost::array<char, BUF_SIZE> > bufPtr(
                new boost::array<char, BUF_SIZE>());
            memcpy(bufPtr->data(), bodyIter->first.data(), bodyIter->second);
            bufVec.push_back(asio::buffer(bufPtr->data(), bodyIter->second));
            bufPtrVec.push_back(bufPtr);
        }
        chunkStr = "\r\n";
        boost::shared_ptr<boost::array<char, BUF_SIZE> > trailerPtr(
                new boost::array<char, BUF_SIZE>());
        memcpy(trailerPtr->data(), chunkStr.c_str(), chunkStr.size());
        bufVec.push_back(asio::buffer(trailerPtr->data(), chunkStr.size()));
        bufPtrVec.push_back(trailerPtr);
        
        asio::async_write(connPtr->getSocket(), bufVec,
                boost::bind(&PubSubManager::handleImgWrite, this,
                        asio::placeholders::error,
                        asio::placeholders::bytes_transferred, bufPtrVec));
    }
}

void PubSubManager::imgSubscribe(std::string sessionToken,
        http::ServerConnectionPtr connPtr)
{
    if (this->imgSubConnMap.find(sessionToken) == this->imgSubConnMap.end())
    {
        this->cmdPublish(sessionToken, CMD_CAMERA_START);    
    }
    
    this->imgSubConnMap.insert(
            std::pair<std::string, http::ServerConnectionPtr>(sessionToken,
                connPtr));
}

void PubSubManager::imgUnsubscribe(std::string sessionToken,
        http::ServerConnectionPtr connPtr)
{
    pair<ConnMap::iterator, ConnMap::iterator> connRange =
            this->imgSubConnMap.equal_range(sessionToken);
    for (ConnMap::iterator connIter = connRange.first;
            connIter != connRange.second; connIter++)
    {
        http::ServerConnectionPtr aConnPtr = connIter->second;
        
        if (aConnPtr == connPtr)
        {
            this->imgSubConnMap.erase(connIter);
            break;
        }
    }
    
    if (this->imgSubConnMap.find(sessionToken) == this->imgSubConnMap.end())
    {
        this->cmdPublish(sessionToken, CMD_CAMERA_STOP);    
    }
}

void PubSubManager::handleWrite(const boost::system::error_code &error,
                     size_t bytesTransferred,
                     boost::shared_ptr<boost::array<char, BUF_SIZE> > bufPtr)
{
}

void PubSubManager::handleImgWrite(const boost::system::error_code &error,
    size_t bytesTransferred,
    std::vector<boost::shared_ptr<boost::array<char, BUF_SIZE> > > bufPtrVec)
{
}

void PubSubManager::cmdPublish(std::string sessionToken, std::string cmd)
{
    pair<ConnMap::iterator, ConnMap::iterator> connRange =
            this->cmdSubConnMap.equal_range(sessionToken);
    for (ConnMap::iterator connIter = connRange.first;
            connIter != connRange.second; connIter++)
    {
        http::ServerConnectionPtr connPtr = connIter->second;
        
        // write chunk length
        std::stringstream ss;
        ss << std::hex << (cmd.size());

        std::string chunkStr = ss.str();
        chunkStr += "\r\n";
        chunkStr += cmd;
        chunkStr += "\r\n";
        boost::shared_ptr<boost::array<char, BUF_SIZE> > bufPtr(
                new boost::array<char, BUF_SIZE>());
        memcpy(bufPtr->data(), chunkStr.c_str(), chunkStr.size());
        asio::async_write(connPtr->getSocket(), asio::buffer(bufPtr->data(),
            chunkStr.size()), boost::bind(&PubSubManager::handleWrite,
                this, asio::placeholders::error,
                asio::placeholders::bytes_transferred, bufPtr));
    }
}

void PubSubManager::cmdSubscribe(std::string sessionToken,
        http::ServerConnectionPtr connPtr)
{
    this->cmdSubConnMap.insert(
            std::pair<std::string, http::ServerConnectionPtr>(sessionToken,
                connPtr));
}

void PubSubManager::cmdUnsubscribe(std::string sessionToken)
{
    this->cmdSubConnMap.erase(sessionToken);
}