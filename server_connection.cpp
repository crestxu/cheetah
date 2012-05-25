#include "server_connection.h"

using namespace http;

boost::shared_ptr<ServerConnection> ServerConnection::create(
        asio::io_service &_ioService, ConnectionManager &_connMgr,
        TokenHandler &_tokenHandler, TokenMonitor &_tokenMonitor,
        Logger &_logger)
{
    return boost::shared_ptr<ServerConnection>(new ServerConnection(_ioService,
        _connMgr, _tokenHandler, _tokenMonitor, _logger));
}

void ServerConnection::start()
{
    this->socket.async_read_some(asio::buffer(this->dataBuf),
        boost::bind(&ServerConnection::handleRead, this,
            asio::placeholders::error,
            asio::placeholders::bytes_transferred));
}

void ServerConnection::stop()
{
    this->socket.close();
}

ServerConnection::ServerConnection(asio::io_service &_ioService,
                ConnectionManager &_connMgr,
                TokenHandler &_tokenHandler, TokenMonitor &_tokenMonitor,
                Logger &_logger) :
        ioService(_ioService),
        socket(_ioService),
        connMgr(_connMgr),
        isSubReq(false),
        isCmdSubReq(false),
        tokenHandler(_tokenHandler),
        tokenMonitor(_tokenMonitor),
        logger(_logger)
{
}

void ServerConnection::handleRead(const boost::system::error_code &error,
                size_t bytesTransferred)
{
    if (!error)
    {
        this->response.contentStream.reset();
        int bytesConsumed = 0;
        boost::tribool result;
        boost::tie(result, bytesConsumed, boost::tuples::ignore) =
            this->requestParser.parse(
                this->request, this->dataBuf.data(),
                this->dataBuf.data() + bytesTransferred);

        if (result)
        {
            // check actual method received
            std::string reqPath("");
            size_t firstSlash = this->request.uri.find_first_of("/");
            size_t lastSlash = this->request.uri.find_last_of("/");
            if (firstSlash != 0 || lastSlash == std::string::npos ||
                lastSlash == this->request.uri.size() - 1)
            {
                this->response = Response::get(Response::BAD_REQUEST);
            } else
            {
                reqPath = this->request.uri.substr(firstSlash+1, lastSlash - 1);                
            }

            // check authentication token
            this->sessionToken = this->request.getValue("Session-Token");
            if (reqPath != reqCrossDomain &&
                !tokenHandler.check(this->sessionToken))
            {
                this->response = http::Response::get(Response::UNAUTHORIZED);
                this->writeBuf.assign('\0');
                size_t len = this->response.serialize(this->writeBuf);
                asio::async_write(this->socket,
                    asio::buffer(this->writeBuf.c_array(), len), boost::bind(
                    &ServerConnection::finish, this, asio::placeholders::error,
                    asio::placeholders::bytes_transferred, true));
                return;
            }

            bool close = true;
            if (this->request.method == "PUT" && reqPath == reqImagePub)
            {
                // respond OK
                this->bodyLength = boost::lexical_cast<int>(
                    this->request.getValue("Content-Length"));
                if (bytesConsumed < bytesTransferred) {
                    // move the read data to body
                    this->readLength = bytesTransferred - bytesConsumed;
                    boost::array<char, BUF_SIZE> newDataBuf;
                    memcpy(newDataBuf.data(), this->dataBuf.data() +
                            bytesConsumed, this->readLength);
                    this->requestBody.insert(newDataBuf, this->readLength);
                } else {
                    this->readLength = 0;
                }
                this->readPutBody();
                return;
            } else if (reqPath == reqImageSub)
            {
                // Subscribe to image
                this->sigImgSub(this->sessionToken,
                        boost::static_pointer_cast<ServerConnection,
                                Connection>(shared_from_this()));
                // respond OK
                this->response = Response::get(Response::OK);
                this->response.setValue("Content-Type", "binary");
                this->response.setValue("Transfer-Encoding", "chunked");
                close = false;
                this->isSubReq = true;
            } else if (reqPath == reqCmdPub)
            {
                // respond OK
                std::string cmd = this->request.getValue("Command");
                this->sigCmdPub(this->sessionToken, cmd);

                this->response = Response::get(Response::OK);
                this->response.setValue("Content-Length", "0");
                close = true;
            } else if (reqPath == reqCmdSub) {
                // subscribe to command
                this->sigCmdSub(this->sessionToken,
                        boost::static_pointer_cast<ServerConnection, Connection>
                                (shared_from_this()));
                // respond OK
                this->response = Response::get(Response::OK);
                this->response.setValue("Content-Type", "text/plain");
                this->response.setValue("Transfer-Encoding", "chunked");
                close = false;
                this->isCmdSubReq = true;
                this->tokenMonitor.untrack(this->sessionToken);
            } else if (reqPath == reqCrossDomain)
            {
                this->response = Response::get(Response::OK);
                this->response.setValue("Content-Type", "application/xml");
                this->response.content = CROSS_DOMAIN_POLICY;
                this->response.setValue("Content-Length",
                    boost::lexical_cast<std::string>(
                        this->response.content.size()));
            } else
            {
                this->response = Response::get(Response::BAD_REQUEST);
                this->response.setValue("Content-Type", "text/plain");
                this->response.setValue("Content-Length",
                    boost::lexical_cast<std::string>(
                        this->response.content.size()));
            }

            this->writeBuf.assign('\0');
            size_t len = this->response.serialize(this->writeBuf);
            asio::async_write(this->socket,
                asio::buffer(this->writeBuf.c_array(), len), boost::bind(
                &ServerConnection::finish, this, asio::placeholders::error,
                asio::placeholders::bytes_transferred, close));
        } else
        {
            // Flash wants policy xml again :(
            std::string reqStr = this->dataBuf.c_array();
            if (reqStr == "<policy-file-request/>")
            {
                std::string policyStr = CROSS_DOMAIN_POLICY;
                this->writeBuf.assign('\0');
                memcpy(this->writeBuf.data(), policyStr.c_str(), policyStr.size());
                asio::async_write(this->socket,
                    asio::buffer(this->writeBuf.c_array(), policyStr.size()), boost::bind(
                    &ServerConnection::finish, this, asio::placeholders::error,
                    asio::placeholders::bytes_transferred, true));
            }
        }
    } else
    {
        if (this->isSubReq)
        {
            this->sigImgUnsub(this->sessionToken,
                    boost::static_pointer_cast<ServerConnection, Connection>
                                (shared_from_this()));
        }
        if (this->isCmdSubReq)
        {
            this->sigCmdUnsub(this->sessionToken);
            // get current time
            posix_time::ptime epoch(gregorian::date(1970, 1, 1));
            posix_time::ptime now = posix_time::second_clock::local_time();
            posix_time::time_duration::sec_type unixTime =
                    (now - epoch).total_seconds();
            this->tokenMonitor.track(this->sessionToken, unixTime);
        }
        this->connMgr.stop(shared_from_this());
    }
}

asio::ip::tcp::socket &ServerConnection::getSocket()
{
    return this->socket;
}

void ServerConnection::finish(const boost::system::error_code &error,
                                size_t bytesTransferred,
                              bool close)
{
    if (this->response.contentStream)
        this->response.contentStream->close();

    if (close)
    {
        if (!error)
        {
            // Shut down connection
            boost::system::error_code ignored;
            this->socket.shutdown(asio::ip::tcp::socket::shutdown_both, ignored);
        }

        if (error != asio::error::connection_aborted)
        {
            this->connMgr.stop(shared_from_this());
        }
    } else
    {
        this->socket.async_read_some(asio::buffer(this->dataBuf),
        boost::bind(&ServerConnection::handleRead, this,
            asio::placeholders::error,
            asio::placeholders::bytes_transferred));
    }
}

void ServerConnection::connectImgSubSignal(boost::function<void
        (std::string, boost::shared_ptr<ServerConnection>)> callback)
{
    this->sigImgSub.connect(callback);
}

void ServerConnection::connectImgUnsubSignal(boost::function<void
        (std::string, boost::shared_ptr<ServerConnection>)> callback)
{
    this->sigImgUnsub.connect(callback);
}

void ServerConnection::connectImgPubSignal(boost::function<void (
                                        std::string, RequestBody &)> callback)
{
    this->sigImgPub.connect(callback);
}

void ServerConnection::readPutBody()
{
    this->dataBuf.assign('\0');
    this->socket.async_read_some(asio::buffer(this->dataBuf),
        boost::bind(&ServerConnection::handleReadPutBody, this,
            asio::placeholders::error,
            asio::placeholders::bytes_transferred));
}

void ServerConnection::handleReadPutBody(
                const boost::system::error_code &error,
                size_t bytesTransferred)
{   
    if (!error)
    {
        this->readLength += bytesTransferred;

        this->requestBody.insert(this->dataBuf, bytesTransferred);

        if (this->readLength < this->bodyLength)
        {
            // read more
            this->readPutBody();
        } else
        {
            // done reading, respond
            this->doneReadPutBody();
        }
    } else
    {
        this->connMgr.stop(shared_from_this());
    }
}

void ServerConnection::doneReadPutBody()
{
    this->response = Response::get(Response::OK);
    this->response.setValue("Content-Length", "0");
    this->response.setValue("Content-Type", "text/plain");

    this->writeBuf.assign('\0');
    size_t len = this->response.serialize(this->writeBuf);
    asio::async_write(this->socket,
        asio::buffer(this->writeBuf.c_array(), len), boost::bind(
        &ServerConnection::finish, this, asio::placeholders::error,
            asio::placeholders::bytes_transferred, true));

    this->sigImgPub(this->sessionToken, this->requestBody);
}

void ServerConnection::connectCmdSubSignal(boost::function<void
    (std::string, boost::shared_ptr<ServerConnection>)> callback)
{
    this->sigCmdSub.connect(callback);
}

void ServerConnection::connectCmdUnsubSignal(boost::function<void
    (std::string)> callback)
{
    this->sigCmdUnsub.connect(callback);
}

void ServerConnection::connectCmdPubSignal(boost::function<void (std::string,
    std::string cmd)> callback)
{
    this->sigCmdPub.connect(callback);
}
