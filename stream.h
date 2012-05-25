#ifndef __STREAM_H__
#define __STREAM_H__

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "stream_exception.h"

namespace cslib 
{

enum StreamCode { SC_SUCCESS, SC_ERROR, SC_ERROR_EOF, 
                  SC_ERROR_OPEN, SC_ERROR_READ, SC_ERROR_WRITE,                   
                  SC_BAD_DESCRIPTOR, SC_OPERATION_ABORTED, SC_CONNECTION_ABORTED, 
                  SC_CONNECTION_RESET, SC_NOT_SUPPORTED };
enum StatusCode { STC_SUCCESS, STC_ERROR, STC_ALREADY_SAVED, STC_BEING_CACHED, STC_PAUSE};
enum SeekType { ST_SEEK_SET, ST_SEEK_CUR, ST_SEEK_END };

typedef boost::function<void (cslib::StreamCode code, int rc)> StreamDoneCb;

class Stream
{
public:
    virtual ~Stream();

    virtual StreamCode read(void *buf, size_t bufLen, size_t *read) = 0;
    virtual StreamCode write(void *buf, size_t bufLen, size_t *written) = 0;
    virtual StreamCode seek(size_t position, SeekType seekType) = 0;
    virtual void close() = 0;
    virtual StreamCode size(size_t *size) const = 0;
    virtual StreamCode rewind() = 0;
    virtual StreamCode getPosition(size_t *pos) = 0;

    virtual void asyncRead(void *buf, size_t bufLen,
        StreamDoneCb readCb) = 0;
    virtual void asyncWrite(void *buf, size_t bufLen,
        StreamDoneCb writeCb) = 0;
};

typedef boost::shared_ptr<Stream> StreamPtr;

}

#endif
