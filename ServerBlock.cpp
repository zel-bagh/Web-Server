#include "File_hpp/ServerBlock.hpp"

ServerBlock& ServerBlock::operator=(const ServerBlock& instance) {
	//std::cout << "Copy ServerBlock assignement operator" << std::endl;
	this->listen = instance.listen;
	this->server_names = instance.server_names;
	this->client_max_body_size = instance.client_max_body_size;
	this->Error_page = instance.Error_page;
	this->root = instance.root;
	this->index = instance.index;
	this->autoindex =  instance.autoindex;
	this->allowed_methods = instance.allowed_methods;   // std::vector<std::string>
	this->locationBlock = instance.locationBlock;
	return *this;
}

ServerBlock::ServerBlock(const ServerBlock& instance) : listen(instance.listen) {
	//std::cout << "Copy ServerBlock Constructor" << std::endl;
	*this = instance;
}

ServerBlock::ServerBlock() {
	//std::cout << "ServerBlock Constructor" << std::endl;
	client_max_body_size = -1;
}

ServerBlock::~ServerBlock() {
	//std::cout << "ServerBlock Destructor" << std::endl;
}