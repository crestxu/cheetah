#include "request_parser.h"

using namespace http;

RequestParser::RequestParser() : state(METHOD_START)
{
}

void RequestParser::reset()
{
    state = METHOD_START;
}

boost::tribool RequestParser::consume(Request &req, char input)
{
    switch (this->state)
    {
    case METHOD_START:
        if (!isChar(input) || isCtl(input) || isTspecial(input))
        {
            return false;
        } else
        {
            this->state = METHOD;
            req.method.push_back(input);
            return boost::indeterminate;
        }
        break;
    case METHOD:
        if (input == ' ')
        {
            this->state = URI;
            return boost::indeterminate;
        } else if (!isChar(input) || isCtl(input) || isTspecial(input))
        {
            return false;
        } else
        {
            req.method.push_back(input);
            return boost::indeterminate;
        }
        break;
    case URI_START:
        if (isCtl(input))
        {
            return false;
        } else
        {
            this->state = URI;
            req.uri.push_back(input);
            return boost::indeterminate;
        }
        break;
    case URI:
        if (input == ' ')
        {
            this->state = HTTP_VERSION_H;
            return boost::indeterminate;
        } else if (isCtl(input))
        {
            return false;
        } else
        {
            req.uri.push_back(input);
            return boost::indeterminate;
        }
        break;
    case HTTP_VERSION_H:
        if (input == 'H')
        {
            this->state = HTTP_VERSION_T_1;
            return boost::indeterminate;
        } else
        {
            return false;
        }
        break;
    case HTTP_VERSION_T_1:
        if (input == 'T')
        {
            this->state = HTTP_VERSION_T_2;
            return boost::indeterminate;
        } else
        {
            return false;
        }
        break;
    case HTTP_VERSION_T_2:
        if (input == 'T')
        {
            this->state = HTTP_VERSION_P;
            return boost::indeterminate;
        } else
        {
            return false;
        }
        break;
    case HTTP_VERSION_P:
        if (input == 'P')
        {
            this->state = HTTP_VERSION_FLASH;
            return boost::indeterminate;
        } else
        {
            return false;
        }
        break;
    case HTTP_VERSION_FLASH:
        if (input == '/')
        {
            req.httpVersionMajor = 0;
            req.httpVersionMinor = 0;
            this->state = HTTP_VERSION_MAJOR_START;
            return boost::indeterminate;
        } else
        {
            return false;
        }
        break;
    case HTTP_VERSION_MAJOR_START:
        if (isDigit(input))
        {
            req.httpVersionMajor = req.httpVersionMajor * 10 + input - '0';
            this->state = HTTP_VERSION_MAJOR;
            return boost::indeterminate;
        } else
        {
            return false;
        }
        break;
    case HTTP_VERSION_MAJOR:
        if (input == '.')
        {
            this->state = HTTP_VERSION_MINOR_START;
            return boost::indeterminate;
        } else if (isDigit(input))
        {
            req.httpVersionMajor = req.httpVersionMajor * 10 + input - '0';
            return boost::indeterminate;
        } else
        {
            return false;
        }
        break;
    case HTTP_VERSION_MINOR_START:
        if (isDigit(input))
        {
            req.httpVersionMinor = req.httpVersionMinor * 10 + input - '0';
            this->state = HTTP_VERSION_MINOR;
            return boost::indeterminate;
        } else
        {
            return false;
        }
        break;
    case HTTP_VERSION_MINOR:
        if (input == '\r')
        {
            this->state = EXPECTING_NEWLINE_1;
            return boost::indeterminate;
        } else if (isDigit(input))
        {
            req.httpVersionMinor = req.httpVersionMinor * 10 + input - '0';
            return boost::indeterminate;
        } else
        {
            return false;
        }
        break;
    case EXPECTING_NEWLINE_1:
        if (input == '\n')
        {
            this->state = HEADER_LINE_START;
            return boost::indeterminate;
        } else
        {
            return false;
        }
        break;
    case HEADER_LINE_START:
        if (input == '\r')
        {
            this->state = EXPECTING_NEWLINE_3;
            return boost::indeterminate;
        } else if (!req.headers.empty() && (input == ' ' || input == '\t'))
        {
            this->state = HEADER_LWS;
            return boost::indeterminate;
        } else if (!isChar(input) || isCtl(input) || isTspecial(input))
        {
            return false;
        } else
        {
            req.headers.push_back(Header());
            req.headers.back().name.push_back(input);
            this->state = HEADER_NAME;
            return boost::indeterminate;
        }
        break;
    case HEADER_LWS:
        if (input == '\r')
        {
            this->state = EXPECTING_NEWLINE_2;
            return boost::indeterminate;
        } else if (input == ' ' || input == '\t')
        {
            return boost::indeterminate;
        } else if (isCtl(input))
        {
            return false;
        } else
        {
            this->state = HEADER_VALUE;
            req.headers.back().value.push_back(input);
            return boost::indeterminate;
        }
        break;
    case HEADER_NAME:
        if (input == ':')
        {
            this->state = SPACE_BEFORE_HEADER_VALUE;
            return boost::indeterminate;
        } else if (!isChar(input) || isCtl(input) || isTspecial(input))
        {
            return false;
        } else
        {
            req.headers.back().name.push_back(input);
            return boost::indeterminate;
        }
        break;
    case SPACE_BEFORE_HEADER_VALUE:
        if (input == ' ')
        {
            this->state = HEADER_VALUE;
            return boost::indeterminate;
        } else return false;
        break;
    case HEADER_VALUE:
        if (input == '\r')
        {
            this->state = EXPECTING_NEWLINE_2;
            return boost::indeterminate;
        } else if (isCtl(input))
        {
            return false;
        } else
        {
            req.headers.back().value.push_back(input);
            return boost::indeterminate;
        }
        break;
    case EXPECTING_NEWLINE_2:
        if (input == '\n')
        {
            this->state = HEADER_LINE_START;
            return boost::indeterminate;
        } else return false;
        break;
    case EXPECTING_NEWLINE_3:
        return (input == '\n');
        break;
    default:
        return false;
    }

    return boost::indeterminate;
}

bool RequestParser::isChar(int c)
{
    return c >= 0 && c <= 127;
}

bool RequestParser::isCtl(int c)
{
    return c >= 0 && c <= 31 || c == 127;
}

bool RequestParser::isTspecial(int c)
{
    switch (c)
    {
    case '(': case ')': case '<': case '>': case '@':
    case ',': case ';': case ':': case '\\': case '"':
    case '/': case '[': case ']': case '?': case '=':
    case '{': case '}': case ' ': case '\t':
        return true;
    default:
        return false;
    }
}

bool RequestParser::isDigit(int c)
{
    return c >= '0' && c <= '9';
}