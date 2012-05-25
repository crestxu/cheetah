#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace http
{

class Connection : public boost::enable_shared_from_this<Connection>
{
public:
    virtual void start() = 0;
    virtual void stop() = 0;
};

typedef boost::shared_ptr<Connection> ConnectionPtr;

}

#endif