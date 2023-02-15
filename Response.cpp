#include "./File_hpp/Response.hpp"

/*
    //	seekg() is used to move the get pointer to a desired location
    //	tellg() is used to know where to get pointer is in a file.

    // How to write std::string to file?
        file <<  std::string str;
*/

Response& Response::operator=(const Response& instance) {
	// std::cout << "Copy assignement operator" << std::endl;
	
    error = instance.error;
    connection = instance.connection;
    return *this;
}

// Response::Response(const Response& instance) : Response(instance.sockid) {
Response::Response(const Response& instance) : sockid(instance.sockid) {
	// std::cout << "Copy Constructor" << std::endl;
	*this = instance;
}

Response::Response(int id) {
    // std::cout << "Default Constructor" << std::endl;
    std::string file;
//
    file = std::to_string(id) + "_response";
    stream.open(file, std::ios::in | std::ios::out | std::ios::trunc);
    stream_index = 0;
    streamLength = 0;
//
    buf_index = -1;
    bufLength = 0;
//
    error = 0;
    connection = 1;
}

Response::~Response() {
    // std::cout << "Destructor" << std::endl;
}


