#ifndef __SERVER_SESSION_H__
#define __SERVER_SESSION_H__

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include <list>
#include <string.h>
#include <sstream>

#include "logger.h"
#include "config.h"

#include "http_service.h"

using namespace cslib;
using namespace boost;
using namespace boost::asio;

class ServerSession
{
public:
    ServerSession(LogLevel level);
    ServerSession(Logger *_logger);
    ~ServerSession();
    
    bool start();
    void run();
    bool stop();

    bool testSession();

protected:
    virtual void initServer();
    virtual std::string getConfigName();

private:
    void handleControlAccept(
        const boost::system::error_code& error,
        boost::shared_ptr<asio::ip::tcp::socket> controlSocket);
    void handleControlRead(const boost::system::error_code& error,
        size_t bytes_transferred,
        boost::shared_ptr<asio::ip::tcp::socket> controlSocket,
        boost::shared_ptr<boost::array<char, BUF_SIZE> > controlMsgPtr);

    bool isRunning;
    unsigned int nodeIdCount;
    bool externLogger;
    string hostname;
    
    Logger *logger;

    boost::program_options::variables_map configMap;
    asio::io_service ioService;
    boost::scoped_ptr<asio::ip::tcp::acceptor> controlAcceptor;
    boost::scoped_ptr<HttpService> httpService;
};

#endif /* __SERVER_SESSION_H__ */
