#include "tools_cpp.hpp"

// Ressource:
// std::getline() : https://m.cplusplus.com/reference/string/string/getline/


int parsingConfigFile(char *arg, std::vector<ServerBlock> &servers)
{
	std::string line;
	std::vector<std::string> tokens;
	std::ifstream config_file;

	config_file.open(arg, std::ifstream::in);
	if (config_file.is_open())
	{
		// use seekg() to go til the end of the file
		config_file.seekg(0, std::ifstream::end);
		if (config_file.tellg() == 0)
		{
			std::cout << "Error: Empty File" << std::endl;
			return (1);
		}
		else
		{
			config_file.seekg(0, std::ifstream::beg); //  beginning of a stream
			while (getline(config_file, line))
			{
				ServerBlock newServer;
				LocationBlock newLocation;

				tokens = split_line_to_tokens(line);
				if (tokens.empty()) // "case of empty line  "
					continue;
				if	(tokens.at(0)[0] == '#')
					continue;
				else if (tokens.at(0).substr(0, 6) == "server") // "server" "{sdf" || "server{ghfs"
				{
					if (parseServer(newServer, tokens, config_file) == 0)
					{
						if (newServer.listen.it_exist == false)
						{
							newServer.listen.port = 8080;
							newServer.listen.host = "127.0.0.1";
						}
						else if (newServer.listen.host.empty())
							newServer.listen.host = "127.0.0.1";
						servers.push_back(newServer);
					}
					else
					{
						std::cout << "Error parsing server\n";
						return (1);
					}
				}
				else
				{	// case LINE WITH SOMETHING ELSE THEN SERVER
					std::cout << "ERROR NO SERVER BLOCK" << std::endl;
					return (1);
				}
			}
		}
	}
	else
	{
		std::cout << "Error: Opening File" << std::endl;
		return (1);
	}
	return (0);
}