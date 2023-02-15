#include "tools_cpp.hpp"

// Ressource:
	// 		Vector.txt
	//		Blocking.txt
	//		setsockopt():	https://www.gta.ufrj.br/ensino/eel878/sockets/setsockoptman.html
	// 		fcntl():		https://www.gta.ufrj.br/ensino/eel878/sockets/fcntlman.html
	//		select():		https://man7.org/linux/man-pages/man2/select.2.html

void	networkProgramming(std::vector<ServerBlock>& servers)
{
	std::map<int, std::vector<ServerBlock> > listeners;

	int maxSock = -1;
	int sockid;

	// Create a "Server Sockets", and stock it in a std::vector<int> listeners
	std::map<ListenDirective, std::vector<ServerBlock> > map_listeners;

	GetListenerServers(servers, map_listeners);
	
	std::map<ListenDirective, std::vector<ServerBlock> >::iterator iter;  // iterator on map_listeners
	for(iter = map_listeners.begin(); iter != map_listeners.end(); ++iter)
	{
		//std::string has a method called "c_str()" that returns a "const char *"

		//std::cout << "Port: " << iter->first.port << " Host: " << iter->first.host << " ==> \n";
		sockid = createSock(iter->first.port, (iter->first.host).c_str());
		
		listeners[sockid] = map_listeners[iter->first];
		
		if (sockid > maxSock)
			maxSock = sockid;
	}

	//std::cout << "Print the Servers who shares the same HOST AND PORT\n";
	/*std::map<int, std::vector<ServerBlock> >::iterator it_h;
	for(it_h = listeners.begin(); it_h != listeners.end(); ++it_h)
	{
		std::cout << "FOR THIS SOCKET LISTENRES: " << it_h->first << "\n";
		std::vector<ServerBlock>::iterator it_s;
		for (it_s = it_h->second.begin(); it_s != it_h->second.end(); ++it_s)
		{
			std::cout << "---------NEW SERVER----------\n";
            	printServer(*it_s);
        	std::cout << "---------END SERVER-----------\n";
		}
	}*/


	// Vector of clients
	std::vector<Client*> Clients;

	// Set of Read
	fd_set ReadSet;
	FD_ZERO(&ReadSet);
	std::map<int, std::vector<ServerBlock> >::iterator it;
	for (it = listeners.begin(); it != listeners.end(); ++it)
	{
		FD_SET(it->first, &ReadSet);
	}
	// Set of Write
	fd_set WriteSet;
	FD_ZERO(&WriteSet);

	while (1)
	{
		fd_set readyToRead = ReadSet;
		fd_set readyToWrite = WriteSet;
		struct timeval tv = {8, 0}; 
		//std::cout << "wait for select\n";
		if (select(maxSock + 1, &readyToRead, &readyToWrite, 0, &tv) == -1)
		{
			std::cout << "select()" << std::endl;
			throw 4;
		}
		//std::cout << "done select\n";
	// 1- Read from socket

		socklen_t addrlen;
		struct sockaddr_in remoteaddr;  // client address
		addrlen = sizeof(remoteaddr);
		
		//1.1- check for new connection (listeners, ReadSet, &Clients)
		std::map<int, std::vector<ServerBlock> >::iterator it_sock;
		for (it_sock = listeners.begin(); it_sock != listeners.end(); ++it_sock)
		{
			int new_con;
			//std::cout << "newcone\n";
			if (FD_ISSET(it_sock->first, &readyToRead))
			{
				// Accept new connection (accept a client who wants to connect to our server)
				if ((new_con = accept(it_sock->first, (struct sockaddr *)&remoteaddr, &addrlen)) != -1)
				{
					/*	Lots of functions block, "accept()" blocks, all the "recv()" functions block
							==> The reason they can do this is because they're allowed to.
   						When you first create the socket descriptor with "socket()",
								the kernel sets it to blocking.
							==> If you don't want a socket to be blocking,
									you have to make a call to "fctnl()".

						fcntl(new_con, F_SETFL, O_NONBLOCK);   // set to non-blocking, also should be used with write() fct
					*/

					FD_SET(new_con, &ReadSet); // add to master Read set
					if (new_con > maxSock)	// keep track of the maximum
					{
						maxSock = new_con;
					}
					Client *new_clt = new Client(new_con);
					new_clt->listen = it_sock->second.at(0).listen;
					new_clt->beginning_time = ft_time_seconds();
					Clients.push_back(new_clt);
				}
			}
		}
		
		//std::cout << "call for read fct\n";
		// 1.2- Receive requests from clients
		readFromSock(map_listeners, Clients, &WriteSet, &readyToRead, &ReadSet);

		//std::cout << "call for write  fct\n";
		// 2- Write into socket
		writeToSock(Clients, &WriteSet, &readyToWrite, &ReadSet);
	}
}



