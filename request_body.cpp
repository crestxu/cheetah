#include "request_body.h"

using namespace http;

RequestBody::RequestBody()
{
    this->size = 0;
}

void RequestBody::insert(boost::array<char, BUF_SIZE> buf, int size)
{
    this->list.push_back(BodyBufPair(buf, size));
    this->size += size;
}