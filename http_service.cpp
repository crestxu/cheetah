#include "http_service.h"

HttpService::HttpService(asio::io_service &_io_service,
                         std::string _hostname,
                         boost::program_options::variables_map &_configMap,
                         Logger &_logger) :
    ioService(_io_service),
    hostname(_hostname),
    configMap(_configMap),
    logger(_logger)
{
}

void HttpService::start()
{
    this->connMgr.reset(new http::ConnectionManager(this->ioService));
    this->pubSubMgr.reset(new PubSubManager());
    this->dbUtils.reset(new DbUtils(this->configMap));
    this->tokenHandler.reset(new TokenHandler(*this->dbUtils));
    
    this->tokenMonitor.reset(new TokenMonitor(this->ioService, *this->dbUtils));
    this->tokenMonitor->start();

    std::ostringstream os;
    os << this->configMap[CONFIG_HTTP_PORT].as<int>();
    asio::ip::tcp::resolver resolver(this->ioService);
    asio::ip::tcp::resolver::query query(asio::ip::tcp::v4(),
                            this->hostname, os.str());
    asio::ip::tcp::resolver::iterator iterator =
                            resolver.resolve(query);
    this->acceptor.reset(new asio::ip::tcp::acceptor(this->ioService,
        iterator->endpoint()));

    this->acceptNewConn();
}

void HttpService::stop()
{
    this->pubSubMgr.reset();
    this->acceptor->close();
    this->acceptor.reset();
    this->tokenHandler.reset();
    this->dbUtils.reset();
    
    this->tokenMonitor->stop();
    this->tokenMonitor.reset();
}

void HttpService::acceptNewConn()
{
    http::ServerConnectionPtr newConn = http::ServerConnection::create(
        this->ioService, *this->connMgr, *this->tokenHandler,
            *this->tokenMonitor, this->logger);
    
    newConn->connectImgSubSignal(boost::bind(&HttpService::imgSubscribe, this,
            _1, _2));
    newConn->connectImgUnsubSignal(boost::bind(&HttpService::imgUnsubscribe,
            this, _1, _2));
    newConn->connectImgPubSignal(boost::bind(&HttpService::imgPublish, this,
            _1, _2));
    
    newConn->connectCmdSubSignal(boost::bind(&HttpService::cmdSubscribe, this,
            _1, _2));
    newConn->connectCmdUnsubSignal(boost::bind(&HttpService::cmdUnsubscribe,
            this, _1));
    newConn->connectCmdPubSignal(boost::bind(&HttpService::cmdPublish, this,
            _1, _2));
    
    this->acceptor->async_accept(newConn->getSocket(), boost::bind(
        &HttpService::handleNewHttpClient, this,
            asio::placeholders::error, newConn));
}

void HttpService::handleNewHttpClient(const boost::system::error_code& error,
        http::ServerConnectionPtr connPtr)
{
    if (!error)
    {
        this->logger.writeLn(LL_INFO, "New local http client connected.");
        this->connMgr->start(connPtr);
    }

    this->acceptNewConn();
}

void HttpService::imgSubscribe(std::string sessionToken,
        http::ServerConnectionPtr connPtr)
{
    this->pubSubMgr->imgSubscribe(sessionToken, connPtr);
}

void HttpService::imgUnsubscribe(std::string sessionToken,
        http::ServerConnectionPtr connPtr)
{
    this->pubSubMgr->imgUnsubscribe(sessionToken, connPtr);
}

void HttpService::imgPublish(std::string sessionToken, http::RequestBody &reqBody)
{
    this->pubSubMgr->imgPublish(sessionToken, reqBody);
}

void HttpService::cmdSubscribe(std::string sessionToken,
        http::ServerConnectionPtr connPtr)
{
    this->pubSubMgr->cmdSubscribe(sessionToken, connPtr);
}

void HttpService::cmdUnsubscribe(std::string sessionToken)
{
    this->pubSubMgr->cmdUnsubscribe(sessionToken);
}

void HttpService::cmdPublish(std::string sessionToken, std::string cmd)
{
    this->pubSubMgr->cmdPublish(sessionToken, cmd);
}