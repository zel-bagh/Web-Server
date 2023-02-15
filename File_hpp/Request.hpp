#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include"chunk.hpp"
#include"request_data.hpp"

class Request
{
    public:
        int sockid;      
        std::string     content;// where u send me request
        std::string     frequest;
        request_data    data;
        chunk           chk;
        unsigned long long int content_length;
        int             transfer_encoding;

        // Canonical Form
        //Request();
        Request(int sockid);
        Request(const Request& obj);
        Request& operator=(const Request& obj);
        ~Request();
};




#endif