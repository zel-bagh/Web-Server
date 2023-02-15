#include "./File_hpp/Request.hpp"

Request::Request(int sockid)
{
    //std::cout << "Request default\n";
    this->sockid = sockid;
    content_length = 0;
    transfer_encoding = 0;
}

Request::~Request()
{ 
    //std::cout << "Request destru\n";
}

Request::Request(const Request& obj) : sockid(obj.sockid)
{
    //std::cout << "Request copy assi\n";
    *this = obj;
}
Request& Request::operator=(const Request& obj)
{
    //std::cout << "Request assi oper\n";
    sockid = obj.sockid;
    content = obj.content;
    frequest = obj.frequest;
    data = obj.data;
    chk = obj.chk;
    content_length = obj.content_length;
    transfer_encoding = obj.transfer_encoding;
    return *this;
}
