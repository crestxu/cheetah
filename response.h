#ifndef __RESPONSE_H__
#define __RESPONSE_H__

#include <vector>
#include <boost/asio.hpp>

#include "header.h"
#include "config.h"
#include "stream.h"

using namespace boost;

namespace http
{

class Response
{
public:
    enum Status
    {
        OK = 200,
        ACCEPTED = 202,
        OK_PARTIAL = 206,
        MULTIPLE_CHOICES = 300,
        MOVED_PERMANENTLY = 301,
        MOVED_TEMPORARILY = 302,
        TEMPORARY_REDIRECT = 307,
        BAD_REQUEST = 400,
        UNAUTHORIZED = 401,
        NOT_FOUND = 404,
        INTERNAL_SERVER_ERROR = 500
    };

    std::vector<http::Header> headers;
    Status status;
    std::string content;
    cslib::StreamPtr contentStream;

    static std::string getStatusString(Status status);
    static std::string getContentString(Status status);
    static Response get(Status status);

    void setValue(std::string name, std::string value);
    std::string getValue(std::string name);
    size_t serialize(boost::array<char, BUF_SIZE> &writeBuf);
};

}

#endif