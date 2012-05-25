#ifndef __SERVER_CONNECTION_H__
#define __SERVER_CONNECTION_H__

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/signals2.hpp>
#include <boost/lexical_cast.hpp>

#include "token_handler.h"
#include "token_monitor.h"

#include "connection.h"
#include "connection_manager.h"
#include "request_parser.h"
#include "request.h"
#include "response.h"
#include "request_body.h"
#include "logger.h"

using namespace boost;

namespace http
{

const std::string reqImagePub = "image_publish";
const std::string reqImageSub = "image_subscribe";
const std::string reqCmdPub = "cmd_publish";
const std::string reqCmdSub = "cmd_subscribe";
const std::string reqCrossDomain = "crossdomain.xml";

const std::string CMD_CAMERA_START = "0";
const std::string CMD_CAMERA_STOP = "1";

class ServerConnection : public Connection
{
public:
    static boost::shared_ptr<ServerConnection> create(
            asio::io_service &_ioService, ConnectionManager &_connMgr,
            TokenHandler &_tokenHandler,
            TokenMonitor &_tokenMonitor,
            Logger &_logger);
    void start();
    void stop();
    asio::ip::tcp::socket &getSocket();

    void connectImgSubSignal(boost::function<void
        (std::string, boost::shared_ptr<ServerConnection>)> callback);
    void connectImgUnsubSignal(boost::function<void
        (std::string, boost::shared_ptr<ServerConnection>)> callback);
    void connectImgPubSignal(boost::function<void (std::string,
        RequestBody &)> callback);
    
    void connectCmdSubSignal(boost::function<void
        (std::string, boost::shared_ptr<ServerConnection>)> callback);
    void connectCmdUnsubSignal(boost::function<void
        (std::string)> callback);
    void connectCmdPubSignal(boost::function<void (std::string,
        std::string)> callback);

private:
    ServerConnection(asio::io_service &_ioService, ConnectionManager &_connMgr,
        TokenHandler &_tokenHandler, TokenMonitor &_tokenMonitor,
            Logger &_logger);
    void handleRead(const boost::system::error_code &error,
                            size_t bytesTransferred);
    void finish(const boost::system::error_code &error,
                size_t bytesTransferred, bool close = true);
    void readPutBody();
    void handleReadPutBody(
                const boost::system::error_code &error,
                size_t bytesTransferred);
    void doneReadPutBody();

    asio::io_service &ioService;
    asio::ip::tcp::socket socket;
    boost::array<char, BUF_SIZE> dataBuf;
    boost::array<char, BUF_SIZE> writeBuf;
    RequestBody requestBody;
    ConnectionManager &connMgr;
    RequestParser requestParser;
    Request request;
    Response response;
    bool isSubReq, isCmdSubReq;
    int bodyLength;
    int readLength;
    std::string sessionToken;
    TokenHandler &tokenHandler;
    TokenMonitor &tokenMonitor;
    Logger &logger;

    boost::signals2::signal<void (std::string sessionToken,
        RequestBody &)> sigImgPub;
    boost::signals2::signal<void (std::string sessionToken,
        boost::shared_ptr<ServerConnection> connPtr)> sigImgSub;
    boost::signals2::signal<void (std::string sessionToken,
        boost::shared_ptr<ServerConnection> connPtr)> sigImgUnsub;
    
    boost::signals2::signal<void (std::string sessionToken,
        std::string cmd)> sigCmdPub;
    boost::signals2::signal<void (std::string sessionToken,
        boost::shared_ptr<ServerConnection> connPtr)> sigCmdSub;
    boost::signals2::signal<void (std::string sessionToken)> sigCmdUnsub;
};

typedef boost::shared_ptr<ServerConnection> ServerConnectionPtr;

}

#endif