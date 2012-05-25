#include "request.h"

using namespace http;

void Request::setValue(std::string name, std::string value)
{
    for (size_t i = 0; i < this->headers.size(); i++)
    {
        if (this->headers[i].name == name)
        {
            this->headers[i].value = value;
            break;
        }
    }
}

std::string Request::getValue(std::string name)
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