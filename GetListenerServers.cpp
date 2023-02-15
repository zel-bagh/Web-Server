#include "tools_cpp.hpp"

// Ressources:

/*	How can I use std::maps with User-defined types as key ?
		==> Cuz Map needs to compare the values, so you need to define "operator < " for your class.
		==> https://stackoverflow.com/questions/1102392/how-can-i-use-stdmaps-with-user-defined-types-as-key
*/

void GetListenerServers(std::vector<ServerBlock>& servers, std::map<ListenDirective, std::vector<ServerBlock> >& listeners)
{
	if (servers.size() > 0)
	{	
		std::vector<ServerBlock>::iterator iter;
		for (iter = servers.begin(); iter != servers.end(); ++iter)
		{
			listeners[(*iter).listen].push_back(*iter);
		}
	}
}
