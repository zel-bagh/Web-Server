#include "File_hpp/ListenDirective.hpp"

ListenDirective& ListenDirective::operator=(const ListenDirective& instance) {
	//std::cout << "Copy ListenDirective assignement operator" << std::endl;
	this->it_exist = instance.it_exist;
	this->host = instance.host;
	this->port = instance.port;
	return *this;
}

ListenDirective::ListenDirective(const ListenDirective& instance) {
	//std::cout << "Copy ListenDirective Constructor" << std::endl;
	*this = instance;
}

ListenDirective::ListenDirective() {
	//std::cout << "ListenDirective Constructor" << std::endl;
	it_exist = false;
	port = -1;
}

ListenDirective::~ListenDirective() {
	//std::cout << "ListenDirective Destructor" << std::endl;
}

bool	ListenDirective::operator<(const ListenDirective& inst) const {
	//std::cout << "ListenDirective Less Operator\n";
	if (port < inst.port || host != inst.host)
		return (true);
	else
		return (false);
}