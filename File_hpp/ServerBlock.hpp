# ifndef SERVERBLOCK_HPP
#define SERVERBLOCK_HPP

#include "LocationBlock.hpp"
#include "ListenDirective.hpp"
#include "tools.hpp"


class ServerBlock {
	public:
		ListenDirective	listen;
		std::vector<std::string> server_names; // std::vector<std::string
		std::map<int, std::string> Error_page; // std::vector<std::string
		std::string root;      // string
		std::vector<std::string>  index;
		std::vector<std::string>	allowed_methods;
		std::string	autoindex;
		long long int	client_max_body_size;
		std::vector<LocationBlock> locationBlock;

		// Canonical form
		ServerBlock();
		~ServerBlock();
		ServerBlock(const ServerBlock& instance);
		ServerBlock& operator=(const ServerBlock& instance);
};

#endif