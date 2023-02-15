#include "./File_hpp/Client.hpp"

const int& Client::getSockid() const {
	return sockid;
}

//	----------------------------------------

Client& Client::operator=(const Client& instance) {
	// std::cout << "Copy assignement operator" << std::endl;
	sockid = instance.sockid;
	request = instance.request;
	response = instance.response;
	return *this;
}

Client::Client(const Client& instance) : sockid(instance.getSockid()), request(instance.request) , response(instance.response) {
	// std::cout << "Copy Constructor" << std::endl;
}

Client::Client(int sock) : sockid(sock), request(sock), response(sock) {
	// std::cout << "Default Constructor" << std::endl;
}

Client::~Client() {
	// std::cout << "Destructor" << std::endl;
};
