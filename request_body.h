#ifndef __REQUEST_BODY_H__
#define __REQUEST_BODY_H__

#include <vector>
#include <utility>

#include <boost/array.hpp>

#include "config.h"

namespace http
{

typedef std::pair<boost::array<char, BUF_SIZE>, int> BodyBufPair;

class RequestBody
{
public:
    RequestBody();
    void insert(boost::array<char, BUF_SIZE> buf, int size);

    std::vector<BodyBufPair> list;
    int size;
};

}

#endif