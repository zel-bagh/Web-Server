#include "tools_cpp.hpp"

void print_mapcgi(std::map<std::string, std::string> cgi)
{
	std::map<std::string, std::string>::iterator it;

	std::cout << "	PRINT CGI: \n";
	for (it = cgi.begin(); it != cgi.end(); ++it)
	{
    	std::cout << "	" << it->first    // string (key)
              << " "
              << it->second   // string's value 
              << " | ";
	}
	std::cout << "\n";
}

void print_map_error(std::map<int, std::string> errors)
{
	std::map<int, std::string>::iterator it;

	std::cout << "	PRINT ERRORS_page: \n";
	for (it = errors.begin(); it != errors.end(); ++it)
	{
    	std::cout << "	" << it->first    // string (key)
              << "  "
              << it->second   // string's value 
              << "  |  ";
	}
	std::cout << "\n";
}

void print_vector(std::vector<std::string> vec)
{
	std::vector<std::string>::iterator iter;

	for(iter = vec.begin(); iter != vec.end(); ++iter)
		std::cout << *iter << " | ";
	std::cout << "\n";
}

void printLocation(LocationBlock& server)
{
	std::cout << "		Client_max_body_size: " << server.client_max_body_size << std::endl;
	//std::cout << "Error_page: " << print_map(server.Error_page) << std::endl;
	std::cout << "		Root: " << server.root << std::endl;

	std::cout << "		autoindex: " << server.autoindex << std::endl;

	std::cout << "		Upload: " << server.upload << std::endl;

	std::cout << "		Redirection: " ;
	print_vector(server.redirection);

	std::cout << "		Index: ";
	print_vector(server.index);
	
	std::cout << "		Allowed_methods: ";
	print_vector(server.allowed_methods);

	std::cout << "	";
	print_map_error(server.Error_page);

	std::cout << "	";
	print_mapcgi(server.cgi_path);
	std::cout << "\n";	
}

void printServer(ServerBlock& server)
{
	if (server.listen.it_exist == true)
	{
		std::cout << "	Host: "<< server.listen.host << std::endl;
		std::cout << "	Port: "<< server.listen.port << std::endl;
	}
	std::cout << "	Server_names: ";
	print_vector(server.server_names);
	std::cout << "	Root: " << server.root << std::endl;
	std::cout << "	Client_max_body_size: " << server.client_max_body_size << std::endl;
	std::cout << "	autoindex: " << server.autoindex << std::endl;
	std::cout << "	Index: ";
	print_vector(server.index);

	print_map_error(server.Error_page);

	std::cout << "\n";

	std::vector<LocationBlock>::iterator iter;
	for (iter = server.locationBlock.begin(); iter !=server.locationBlock.end(); ++iter)
	{
		std::cout << "	---------NEW Location ----------\n";
		printLocation(*iter);
		std::cout << "	---------END Location -----------\n";
	}
	std::cout << "\n";
}
