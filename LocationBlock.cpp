#include "File_hpp/LocationBlock.hpp"

LocationBlock& LocationBlock::operator=(const LocationBlock& instance) {
	//std::cout << "Copy LocationBlock assignement operator" << std::endl;
	this->path = instance.path;
	this->autoindex = instance.autoindex;
	this->client_max_body_size = instance.client_max_body_size;
	this->Error_page = instance.Error_page; // string
	this->root = instance.root;      // string
	this->index = instance.index;   // std::vector<std::string>
	this->allowed_methods = instance.allowed_methods;   // std::vector<std::string>
	this->upload = instance.upload;
	this->redirection = instance.redirection;
	this->cgi_path = instance.cgi_path;
	return *this;
}

LocationBlock::LocationBlock(const LocationBlock& instance) {
	//std::cout << "Copy LocationBlock Constructor" << std::endl;
	*this = instance;
}

LocationBlock::LocationBlock() {
	//std::cout << "LocationBlock Constructor" << std::endl;
	client_max_body_size = -1;
}

LocationBlock::~LocationBlock() {
	//std::cout << "LocationBlock Destructor" << std::endl;
}
