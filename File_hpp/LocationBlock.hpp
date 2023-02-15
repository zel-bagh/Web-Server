# ifndef LOCATIONBLOCK_HPP
#define LOCATIONBLOCK_HPP

#include "tools.hpp"

class LocationBlock {
	public:
		std::string path;
		std::string	autoindex;
		long long int	client_max_body_size;  // maybe yes
		std::map<int, std::string> Error_page; // string
		std::string root;      // string
		std::vector<std::string>	index;   // std::vector<std::string>
		std::string	upload;
		std::vector<std::string>	allowed_methods;
		std::vector<std::string>	redirection;
		std::map<std::string, std::string> cgi_path;

		// Canonical form
		LocationBlock();
		~LocationBlock();
		LocationBlock(const LocationBlock& instance);
		LocationBlock& operator=(const LocationBlock& instance);
};

#endif