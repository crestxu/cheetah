#ifndef __HTTP_SERVICE_H__
#define __HTTP_SERVICE_H__

#include <string>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>

#include "logger.h"
#include "server_connection.h"
#include "connection_manager.h"
#include "pub_sub_manager.h"
#include "token_handler.h"
#include "token_monitor.h"
#include "db_utils.h"

using namespace boost;

class HttpService
{
public:
    HttpService(asio::io_service &_io_service,
        std::string _hostname, boost::program_options::variables_map &_configMap,
        Logger &_logger);

    void start();
    void stop();

private:
    void acceptNewConn();
    void handleNewHttpClient(const boost::system::error_code& error,
            http::ServerConnectionPtr connPtr);
    void imgSubscribe(std::string sessionToken, http::ServerConnectionPtr connPtr);
    void imgUnsubscribe(std::string sessionToken,
                http::ServerConnectionPtr connPtr);
    void imgPublish(std::string sessionToken, http::RequestBody &reqBody);
    
    void cmdSubscribe(std::string sessionToken,
                http::ServerConnectionPtr connPtr);
    void cmdUnsubscribe(std::string sessionToken);
    void cmdPublish(std::string sessionToken, std::string cmd);

    Logger &logger;
    std::string hostname;
    asio::io_service &ioService;
    boost::scoped_ptr<asio::ip::tcp::acceptor> acceptor;
    boost::scoped_ptr<http::ConnectionManager> connMgr;
    boost::scoped_ptr<PubSubManager> pubSubMgr;
    boost::program_options::variables_map &configMap;
    boost::scoped_ptr<DbUtils> dbUtils;
    boost::scoped_ptr<TokenHandler> tokenHandler;
    boost::scoped_ptr<TokenMonitor> tokenMonitor;
};

#endif