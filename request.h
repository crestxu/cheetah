#ifndef __REQUEST_H__
#define __REQUEST_H__

#include <string>
#include <vector>

#include "header.h"

namespace http
{

struct Request
{
    std::string method;
    std::string uri;
    int httpVersionMajor;
    int httpVersionMinor;
    std::vector<http::Header> headers;

    void setValue(std::string name, std::string value);
    std::string getValue(std::string name);
};

}

#endif