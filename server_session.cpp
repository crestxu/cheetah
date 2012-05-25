#include "server_session.h"

using namespace std;

ServerSession::ServerSession(LogLevel level)
{
    this->logger = new Logger("cheetah.log");
    this->logger->setLogLevel(level);
    this->externLogger = false;

    this->hostname = boost::asio::ip::host_name();
}

ServerSession::ServerSession(Logger *_logger)
{
    this->logger = _logger;
    this->externLogger = true;

    this->hostname = boost::asio::ip::host_name();
}

ServerSession::~ServerSession()
{
    if (!this->externLogger)
    {
        delete this->logger;
    }
}

std::string ServerSession::getConfigName()
{
    return "cheetah.cfg";
}

void ServerSession::initServer()
{
    std::string configName = this->getConfigName();

    boost::program_options::options_description configDesc;
    configDesc.add_options()
        (CONFIG_SSL_PEM, boost::program_options::value<std::string>(),
            "fullpath of ssl private key file")
        (CONFIG_SSL_TMP_DH, boost::program_options::value<std::string>(),
            "fullpath of ssl temp dh file")
        (CONFIG_HTTP_PORT, boost::program_options::value<int>(),
            "port to server http request")
        (CONFIG_CONTROL_PORT, boost::program_options::value<int>(),
            "port to handle external control request to cheetah")
        (CONFIG_DB_HOST,
            boost::program_options::value<std::string>(),
            "Hostname of cougar database")
        (CONFIG_DB_USERNAME,
            boost::program_options::value<std::string>(),
            "Username to access cougar database")
        (CONFIG_DB_PASSWORD,
            boost::program_options::value<std::string>(),
            "Password to access cougar database")
        (CONFIG_DB_NAME,
            boost::program_options::value<std::string>(),
            "Name of cougar database")
            ;
#ifdef WIN32
    // Current dir is not reliable, get directory where
    // executable is running
    char path[MAX_PATH + 1];
    memset(path, 0, MAX_PATH + 1);
    GetModuleFileName(NULL, path, MAX_PATH + 1 );
    std::string pathStr = path;
    boost::filesystem2::path boostPath(pathStr);
    boost::filesystem2::path configPath(boostPath.parent_path());
    configPath /= configName;
    std::string configFullname = configPath.string();

    boost::program_options::store(
        boost::program_options::parse_config_file<char>(
            configFullname.c_str(), configDesc), this->configMap);
#else
    std::ifstream ss(configName.c_str());
    boost::program_options::store(
        boost::program_options::parse_config_file<char>(
            ss, configDesc), this->configMap);
#endif
    boost::program_options::notify(this->configMap);
}

bool ServerSession::start()
{
    this->isRunning = true;

    this->initServer();

    this->controlAcceptor.reset(new asio::ip::tcp::acceptor(
            this->ioService, asio::ip::tcp::endpoint(
                asio::ip::address::from_string(SERVER_ADDR),
                this->configMap[CONFIG_CONTROL_PORT].as<int>())));
    boost::shared_ptr<asio::ip::tcp::socket> controlSocket(
            new asio::ip::tcp::socket(this->ioService));
    this->controlAcceptor->async_accept(*controlSocket, boost::bind(
        &ServerSession::handleControlAccept, this, asio::placeholders::error,
                controlSocket));

    this->httpService.reset(new HttpService(this->ioService, this->hostname,
        this->configMap, *this->logger));
    this->httpService->start();

    return true;
}

void ServerSession::run()
{
    this->ioService.run();
}

bool ServerSession::stop()
{
    bool ret = true;

    this->isRunning = false;

    this->controlAcceptor->close();
    this->controlAcceptor.reset();

    this->httpService->stop();
    this->httpService.reset();

    return ret;
}

bool ServerSession::testSession()
{
    boost::asio::io_service service;
    std::ostringstream os;
    os << this->configMap[CONFIG_HTTP_PORT].as<int>();
    ip::tcp::resolver resolver(service);
    ip::tcp::resolver::query query(asio::ip::tcp::v4(),
                    this->hostname, os.str());
    ip::tcp::resolver::iterator iterator =
                    resolver.resolve(query);
    boost::asio::ip::tcp::socket testSocket(service);

    // connect to client network to test
    try
    {
        testSocket.connect(*iterator);
        testSocket.close();
    } catch (boost::system::system_error)
    {
        try
        {
            testSocket.connect(*iterator);
            testSocket.close();
        } catch (boost::system::system_error)
        {
            return false;
        }
    }

    return true;
}

void ServerSession::handleControlAccept(
        const boost::system::error_code& error,
        boost::shared_ptr<asio::ip::tcp::socket> controlSocket)
{
    boost::shared_ptr<boost::array<char, BUF_SIZE> > controlMsgPtr(
                            new boost::array<char, BUF_SIZE>());
    controlSocket->async_read_some(asio::buffer(controlMsgPtr->data(),
                                    BUF_SIZE), boost::bind(
            &ServerSession::handleControlRead, this, asio::placeholders::error,
            asio::placeholders::bytes_transferred,
            controlSocket, controlMsgPtr));

    boost::shared_ptr<asio::ip::tcp::socket> newControlSocket(
            new asio::ip::tcp::socket(this->ioService));
    this->controlAcceptor->async_accept(*newControlSocket, boost::bind(
        &ServerSession::handleControlAccept, this, asio::placeholders::error,
                newControlSocket));
}

void ServerSession::handleControlRead(const boost::system::error_code& error,
        size_t bytes_transferred,
        boost::shared_ptr<asio::ip::tcp::socket> controlSocket,
        boost::shared_ptr<boost::array<char, BUF_SIZE> > controlMsgPtr)
{
    if (!error && bytes_transferred > 1)
    {
        std::string commandStr = controlMsgPtr->data();
        boost::trim(commandStr);

        if (commandStr == std::string(CMD_STOP))
        {
            this->ioService.stop();
        } else
        {
            // close connection if it's a command we don't understand
            controlSocket->close();
        }
    } else
    {
        controlSocket->close();
    }
}