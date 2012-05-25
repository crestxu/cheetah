#include <boost/lexical_cast.hpp>

#include "response.h"

using namespace http;

namespace StatusString
{
const std::string ok = "HTTP/1.1 200 OK\r\n";
const std::string accepted = "HTTP/1.1 202 ACCEPTED\r\n";
const std::string badRequest = "HTTP/1.1 400 Bad Request\r\n";
const std::string unauthorized = "HTTP/1.1 401 Unauthorized\r\n";
const std::string notFound = "HTTP/1.1 404 Not Found\r\n";
const std::string internalServerError = "HTTP/1.1 500 Internal Server Error\r\n";
}

namespace ContentString
{
    const std::string ok = "";
    const std::string badRequest =
            "<html>"
            "<head><title>Bad Request</title></head>"
            "<body><h1>400 Bad Request</h1></body>"
            "</html>";
    const std::string unauthorized =
            "<html>"
            "<head><title>Unauthorized</title></head>"
            "<body><h1>401 Unauthorized</h1></body>"
            "</html>";
    const std::string notFound =
            "<html>"
            "<head><title>Not Found</title></head>"
            "<body><h1>404 Not Found</h1></body>"
            "</html>";
    const std::string internalServerError =
            "<html>"
            "<head><title>Internal Server Error</title></head>"
            "<body><h1>500 Internal Server Error</h1></body>"
            "</html>";
}

std::string Response::getStatusString(Response::Status status)
{
    switch (status)
    {
    case OK:
        return StatusString::ok;
    case ACCEPTED:
        return StatusString::accepted;
    case BAD_REQUEST:
        return StatusString::badRequest;
    case UNAUTHORIZED:
        return StatusString::unauthorized;
    case NOT_FOUND:
        return StatusString::notFound;
    case INTERNAL_SERVER_ERROR:
        return StatusString::internalServerError;
    default:
        return StatusString::internalServerError;
    }
}

std::string Response::getContentString(Response::Status status)
{
    switch (status)
    {
    case OK:
        return ContentString::ok;
    case BAD_REQUEST:
        return ContentString::badRequest;
    case UNAUTHORIZED:
        return ContentString::unauthorized;
    case NOT_FOUND:
        return ContentString::notFound;
    case INTERNAL_SERVER_ERROR:
        return ContentString::internalServerError;
    default:
        return ContentString::internalServerError;
    }
}

Response Response::get(Status status)
{
    Response res;
    res.status = status;
    res.content = Response::getContentString(status);
    res.headers.resize(1);
    res.headers[0].name = "Server";
    res.headers[0].value = "Cheetah";
    return res;
}

size_t Response::serialize(boost::array<char, BUF_SIZE> &writeBuf)
{
    const std::string nameValueSep = ": ";
    const std::string crlf = "\r\n";
    size_t counter = 0;
    std::string statusStr = Response::getStatusString(this->status);
    memcpy(writeBuf.c_array() + counter, statusStr.c_str(), statusStr.size());
    counter += statusStr.size();
    for (size_t i = 0; i < this->headers.size(); i++)
    {
        http::Header &header = this->headers[i];
        memcpy(writeBuf.c_array() + counter, header.name.c_str(),
                header.name.size());
        counter += header.name.size();
        memcpy(writeBuf.c_array() + counter, nameValueSep.c_str(),
                nameValueSep.size());
        counter += nameValueSep.size();
        memcpy(writeBuf.c_array() + counter, header.value.c_str(),
                header.value.size());
        counter += header.value.size();
        memcpy(writeBuf.c_array() + counter, crlf.c_str(),
                crlf.size());
        counter += crlf.size();
    }

    memcpy(writeBuf.c_array() + counter, crlf.c_str(),
            crlf.size());
    counter += crlf.size();
    memcpy(writeBuf.c_array() + counter, content.c_str(),
            content.size());
    counter += content.size();

    return counter;
}

void Response::setValue(std::string name, std::string value)
{
    size_t i = 0;
    for (; i < this->headers.size(); i++)
    {
        if (this->headers[i].name == name)
        {
            this->headers[i].value = value;
            break;
        }
    }

    if (i == this->headers.size())
    {
        this->headers.resize(i + 1);
        http::Header header;
        header.name = name;
        header.value = value;
        this->headers[i] = header;
    }
}

std::string Response::getValue(std::string name)
{
    for (size_t i = 0; i < this->headers.size(); i++)
    {
        if (this->headers[i].name == name)
        {
            return this->headers[i].value;
        }
    }

    return "";
}