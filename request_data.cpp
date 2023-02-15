#include "./File_hpp/request_data.hpp"

request_data::request_data()
{}
request_data::~request_data(){}
request_data::request_data(const request_data& obj)
{
    *this = obj;
}
request_data& request_data::operator=(const request_data& obj)
{
    method = obj.method;
    request_target = obj.request_target;
    query = obj.query;
    headers = obj.headers;
    bodypath = obj.bodypath;
    location = obj.location;
    return *this;
}