#include "tools_cpp.hpp"

int parseLocation(LocationBlock& newLocation, std::vector<std::string>& tokens, std::ifstream& config_file)
{
	std::string line;
	std::vector<std::string> loc;

	if (checkForAcoladeLocation(tokens, newLocation, config_file) == 0) // CHECK ACOLADE OF location
	{
		if (newLocation.path[0] != '/')	     // Check for Absolute Path
			return (1);
		//std::cout << " ===> SUCCESS FINDING the first acolade of LOCATION\n";
		if (tokens.empty() == false) // "Path{redirection google.com;"
		{
			if (fill_location(tokens, newLocation)) // error == 1
				return (1);
			/*std::vector<std::string>::iterator iter;
			std::cout << "------Begining of LINE of PATH in LOCATION--------\n";
			for (iter = tokens.begin(); iter != tokens.end(); ++iter)
			{
				std::cout << *iter << std::endl;
			}
			std::cout << "------END of LINE TO PATH in LOCATION--------\n";*/
		}
		while (getline(config_file, line))
		{
			tokens = split_line_to_tokens(line);//
			if (tokens.empty())
				continue ;
			if (tokens.at(0)[0] == '#')
				continue ;
			if (tokens.size() == 1 && tokens.at(0) == "}")
			{
				//std::cout << " ===> FIND the second acolade of Location Block\n";
				return (0);
			}
			tokens = split_line_to_tokens(line);// ; should be removed before split
			if (fill_location(tokens, newLocation))	// error == 1
			{
				//std::cout << "error normal directive in location block\n";
				return (1);
			}
		}
	}
	return (1);	
}