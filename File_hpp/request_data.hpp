#ifndef REQUESTDATA_HPP
#define REQUESTDATA_HPP	

#include <string>
#include <map>

#include "ServerBlock.hpp"

class request_data
{
    public:
        std::string                           method;
        std::string                           request_target;
        std::string                           query;
        std::map<std::string, std::string>    headers;
        std::string                           bodypath;
        LocationBlock                         location;
        ServerBlock*                           server;

        // Canonical Form
        request_data();
        request_data(const request_data& obj);
        request_data& operator=(const request_data& obj);
        ~request_data();
};
#endif