#include "tools_cpp.hpp"

int checkForAcoladeServ(std::vector<std::string>& tokens, std::ifstream& conf_file) // I WILL CHANGE IT FOR {
{
	 std::string line;
	 std::vector<std::string> new_tokens;

	//std::cout << "Check acolade for server" << std::endl;
	if (tokens.at(0).length() == 6)  // "server" "{dfgdfgdf"  || "server"     "{fsdfsdf"
	{
		if (tokens.size() == 1)
		{
			//std::cout << "SERVER in a single line\n";
			tokens.erase(tokens.begin());   // erase "server"
			//tokens.at(0).erase(); // erase line:"server" | line: "{"  newline "{dsfsdfsdf"
			getline(conf_file, line);
			if (line.empty() == true)
			{
				//std::cout << "Error empty line after server\n";
				return (1);
			}
			new_tokens = split_line_to_tokens(line);
			if (new_tokens.at(0)[0] == '{')   
			{
				if (new_tokens.size() == 1)
				{
					//std::cout << "ACOLADE in a single line\n";
					new_tokens.erase(new_tokens.begin());   // erase "server"
					tokens = new_tokens;
				}
	 			else //  "{listen 80;"
				{
					//std::cout << "L9AYNA ACOLADE m3a server directive\n";
					new_tokens.at(0) = new_tokens.at(0).substr(1, new_tokens.at(0).length() - 1);
					tokens = new_tokens;
				}
	 		}
	 		else // can't find accolade
			{
	 			return (1);
			}
		}
	 	else if (tokens.at(1)[0] == '{')   
		{
	 		if (tokens.at(1) == "{")  // "server" "{" "dfsdfsd"
	 		{
				tokens.erase(tokens.begin());  // erase "server"
				tokens.erase(tokens.begin());	// erase "{"
	 		}
			else // "server" "{FDSDFS"
			{
	 			tokens.at(1) = tokens.at(1).substr(1, tokens.at(1).length() - 1);
				tokens.erase(tokens.begin());
			}
	 	}
	 	else
		{
			//std::cout << "5tchi\n";
	 		return (1);
		}
	}
	else if (tokens.at(0).length() >= (6 + 1) && tokens.at(0)[6] == '{') // "server{"  || "server{sdfsd"
	{
	 	if (tokens.at(0).length() == (6 + 1))  // "server{" "dsfd"
	 		tokens.erase(tokens.begin());
	 	else	// "server{FDSDFS"
	 		tokens.at(0) = tokens.at(0).substr((6 + 1), tokens.at(0).length() - (6 + 1));
	}
	else
	{
		//std::cout << "wlh ma3raft\n";
	 	return (1);
	}
	return (0);
}