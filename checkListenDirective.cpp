#include "tools_cpp.hpp"

int    is_vector_number(std::vector<std::string> listen_tmp)
{
    std::vector<std::string>::iterator iter;
	for (iter = listen_tmp.begin(); iter != listen_tmp.end(); ++iter)
	{
        if(is_string_number(*iter) == 0 && (*iter).length() <=3)
			continue ;
		else
			return (1);
	}
    return(0);
}

// port available: 1024 49151
int checkport(std::string& str)
{
	int port;

	if (is_string_number(str))
		return (-1);
	if (str.length() > 6)   // check length
		return (-1);
	port = atoi(str.c_str());
	if ( port >= 1 && port <= 49151)
		return (port);
	return (-1);
}

int	checkhost(std::string str)
{
	std::vector<std::string> listen_tmp;

    if (find_del(str, '.') > 3)
        return (1);
    listen_tmp = split_token_with_del(str, '.');
    if (listen_tmp.size() != 4)
        return (1);
    if(is_vector_number(listen_tmp))
		return (1);
	return (0);
}

int checkListenDirective(std::vector<std::string>& tokens, ServerBlock& newServer)
{
	std::vector<std::string> tmp;

	int port;

    if (newServer.listen.it_exist == false && tokens.size() == 2)
	{
		newServer.listen.it_exist = true;  // True
		if (find_del(tokens.at(1), ':') > 1)
		{
			return (1);
		}
		if (find_del(tokens.at(1), ':') == 0)
		{
			port = checkport(tokens.at(1));
			if (port < 0)
				return (1);
			newServer.listen.port = port;
			return (0);
		}
		tmp = split_token_with_del(tokens.at(1), ':');
		if (checkhost(tmp.at(0)))
			return (1);
		newServer.listen.host = tmp.at(0);
		port = checkport(tmp.at(1));
		if (port < 0)
			return (1);
		newServer.listen.port = port;
    }
    else
    	return(1);
	return (0);
}