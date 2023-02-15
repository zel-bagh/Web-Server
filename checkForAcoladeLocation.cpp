#include "tools_cpp.hpp"

int checkForAcoladeLocation(std::vector<std::string>& tokens, LocationBlock& newLocation, std::ifstream& conf_file)
{
	std::string line;
	std::vector<std::string> new_tokens;
	size_t len;

	//std::cout << "CHECK LOCATION acolade fct tokens = " << tokens.at(0) << std::endl;
	len = tokens.at(0).find('{');   // len =  pos of  '{'
	if (len < tokens.at(0).size() && tokens.at(0).length() >= (len + 1) && tokens.at(0)[len] == '{') // "PATH{"  || "PATH{sdfsd"
	{
		//std::cout << " ACOLADE of Location is IN THE SAME LINE OF PATH\n";
	 	if (tokens.at(0).length() == (len + 1))  // "PATH{" "dsfd"
		{
			tokens.at(0) = tokens.at(0).substr(0, len -1);  // erase acolade from "PATH{" == "PATH"
			newLocation.path = tokens.at(0);
	 		tokens.erase(tokens.begin());
		}
	 	else	// "PATH{FDSDFS"
		{
			newLocation.path = tokens.at(0).substr(0, len -1);  // I store the "PATH"
	 		tokens.at(0) = tokens.at(0).substr((len + 1), tokens.at(0).length() - (len + 1)); // tokens = 
		}
	}
	else if (len == std::string::npos && tokens.size() > 1 && tokens.at(1)[0] == '{')   
	{
		//std::cout << " ACOLADE of Location is IN THE SAME LINE OF PATH but acolade in other token\n";
		if (tokens.at(1) == "{")  // "PATH" "{" "dfsdfsd"
	 	{
			newLocation.path = tokens.at(0); // STORE THE PATH
			tokens.erase(tokens.begin());  // erase "PATH"
			tokens.erase(tokens.begin());	// erase "{"
	 	}
		else // "PATH" "{FDSDFS"
		{
			newLocation.path = tokens.at(0); // STORE THE PATH
	 		tokens.at(1) = tokens.at(1).substr(1, tokens.at(1).length() - 1);
			tokens.erase(tokens.begin());  // erase PATH
		}
	}	
	else	// line:PATH   && line: {fsdf
	{
		//std::cout << "acolade of location is in the new line\n";
		if (tokens.size() == 1)   
		{
			newLocation.path = tokens.at(0);
			tokens.erase(tokens.begin()); // erase PATH
			getline(conf_file, line);  // "{SDFSDF"
			if (line.empty() == true)
				return (1);
			new_tokens = split_line_to_tokens(line);
			if (new_tokens.at(0)[0] == '{')   // "{SDFSDF"	
			{
	 			if (new_tokens.at(0) == "{")  //  "{" "directive"
	 				new_tokens.erase(new_tokens.begin());   // erase "{"
	 			else //  "{listen 80;"
				{
					new_tokens.at(0) = new_tokens.at(1).substr(1, new_tokens.at(1).length() - 1);
					tokens = new_tokens;
				}
	 		}
	 		else
			{
				//std::cout << "Can't find Acolade of location\n";
	 			return (1);
			}
		}
	 	else		// > 1
		{
			//std::cout << "Can't over \n";
	 		return (1);
		}
	}
	return (0);
}
