#include "tools_cpp.hpp"

void	readFromSock(std::map<ListenDirective, std::vector<ServerBlock> > map_listeners, std::vector<Client*>& Clients, fd_set* WriteSet, fd_set* readyToRead, fd_set *ReadSet)
{
	std::vector<Client*>::iterator iter_client;
	
	if (Clients.size() > 0)
	{
		for(iter_client = Clients.begin(); iter_client != Clients.end(); ++iter_client)
		{
			//std::cout << "reading rquest zak part\n";
			if (FD_ISSET((*iter_client)->getSockid(), readyToRead))
			{
				char request_buf[1024];
				size_t nbytes;

				// receive data from a client

				// In case of recv == 0, For TCP sockets, the return value 0 means the peer has closed its half side of the connection.
				if ((nbytes = recv((*iter_client)->getSockid(), request_buf, 1024, 0)) <= 0)
				{
					// in case of nbytes == -1 : remove the client
					FD_CLR((*iter_client)->getSockid(), ReadSet);
					FD_CLR((*iter_client)->getSockid(), readyToRead);
					
					// remove the client
					close((*iter_client)->getSockid());
					delete (*iter_client);   // new
					Clients.erase(iter_client);
					readFromSock(map_listeners, Clients, WriteSet, readyToRead, ReadSet);// recursive, fix the segfault problem
					break;
				}
				else
				{
					(*iter_client)->request.content = std::string(request_buf, nbytes);

////////////////////////////////////////////////
				//std::cout << "PRINTING LISTENERS, socket: " << listeners[(*iter_client)->getSockid()] << std::endl;
				// std::vector<ServerBlock>::iterator it_s;
				// for (it_s = map_listeners[(*iter_client)->listen].begin(); it_s != map_listeners[(*iter_client)->listen].end(); ++it_s)
				// {
				// 	std::cout << "---------NEW SERVER----------\n";
            	// 	printServer(*it_s);
        		// 	std::cout << "---------END SERVER-----------\n";
				// }
//////////////////////////////////////////////

					if (parseRequest(*(*iter_client), map_listeners[(*iter_client)->listen]) == 1) // == 1, end of request
					{
						FD_CLR((*iter_client)->getSockid(), ReadSet);
						FD_CLR((*iter_client)->getSockid(), readyToRead);
						// Generate response:
						treatRequest(*(*iter_client));   // generate the accurete response
						FD_SET((*iter_client)->getSockid(), WriteSet);
					}
				}
			}
			else
			{
				size_t limmit = 6;
				if ((ft_time_seconds() - (*iter_client)->beginning_time) > limmit)
				{
					//std::cout << "error 408 timeout\n";
					FD_CLR((*iter_client)->getSockid(), ReadSet);
					FD_CLR((*iter_client)->getSockid(), readyToRead);

					try {
					genereate_response(*(*iter_client), 408, "", 1, "", 0);
					}
					catch (...){}
					FD_SET((*iter_client)->getSockid(), WriteSet);
				}
			}
		}
	}
}