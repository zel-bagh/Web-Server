#include "tools_cpp.hpp"

int parseServer(ServerBlock& newServer, std::vector<std::string>& tokens, std::ifstream& config_file)
{
	std::string line;

	if (checkForAcoladeServ(tokens, config_file) == 0)  // find acolade of server
	{
		//std::cout << " ===> SUCCESS FINDING the first ACOLADE OF SERVER" << std::endl;
		if (tokens.empty() == false) // "server{listen 80;"
		{
			if (fill_server(tokens, newServer)) // error == 1
				return (1);

			/*std::cout << "---------DIRECTIVE WITH THE SERVER{ " << std::endl;
			std::vector<std::string>::iterator iter;
			for (iter = tokens.begin(); iter != tokens.end(); ++iter)
				std::cout << *iter << " ";
			std::cout << "\n------END DIRECTIVE TO SERVER--------\n";
			*/
		}
		while (getline(config_file, line)) // loop inside server block;
		{
			tokens = split_line_to_tokens(line);//
			if (tokens.empty())
				continue ;
			if (tokens.at(0)[0] == '#')
				continue ;
			if (tokens.size() == 1 && tokens.at(0) == "}")
			{
				//std::cout << " ===> Find the second acolade of SERVER Block\n";
				return (0);
			}
			if (fill_server(tokens, newServer)) // error  == 1
			{
				if (tokens.at(0) == "location")
				{
					//std::cout << "It's Location directive\n";
					LocationBlock newLocation;

					tokens.erase(tokens.begin());	// I erase the location token from the vector
					if (!parseLocation(newLocation, tokens, config_file))
						newServer.locationBlock.push_back(newLocation);
					else
					{
						//std::cout << "ERROR in location block";
						return (1);
					}
				}
				else
				{
					//std::cout << "Error directive in Server block\n";
					return (1);
				}
			}
		}
	}
	return (1);
}