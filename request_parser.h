#ifndef __REQUEST_PARSER_H__
#define __REQUEST_PARSER_H__

#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>

#include "request.h"

namespace http
{

class RequestParser
{
public:
    RequestParser();

    // Reset parser to initial state
    void reset();

    template <typename InputIterator>
    boost::tuple<boost::tribool, int, InputIterator> parse(Request &req,
        InputIterator begin, InputIterator end)
    {
        int byteConsumed = 0;
        while (begin != end)
        {
            boost::tribool result = consume(req, *begin++);
            byteConsumed++;
            if (result || !result)
                return boost::make_tuple(result, byteConsumed, begin);
        }
        boost::tribool result = boost::indeterminate;
        return boost::make_tuple(result, byteConsumed, begin);
    }

private:
    boost::tribool consume(Request &req, char input);
    bool isChar(int c);
    bool isCtl(int c);
    bool isTspecial(int c);
    bool isDigit(int c);

    enum State
    {
        METHOD_START,
        METHOD,
        URI_START,
        URI,
        HTTP_VERSION_H,
        HTTP_VERSION_T_1,
        HTTP_VERSION_T_2,
        HTTP_VERSION_P,
        HTTP_VERSION_FLASH,
        HTTP_VERSION_MAJOR_START,
        HTTP_VERSION_MAJOR,
        HTTP_VERSION_MINOR_START,
        HTTP_VERSION_MINOR,
        EXPECTING_NEWLINE_1,
        HEADER_LINE_START,
        HEADER_LWS,
        HEADER_NAME,
        SPACE_BEFORE_HEADER_VALUE,
        HEADER_VALUE,
        EXPECTING_NEWLINE_2,
        EXPECTING_NEWLINE_3
    };

    enum State state;
};

}

#endif