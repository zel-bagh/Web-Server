#include "tools_cpp.hpp"

// Ressource: https://www.gta.ufrj.br/ensino/eel878/sockets/syscalls.html

int	createSock(int port, const char* addr)
{
	struct sockaddr_in my_addr;
	int sockfd;
	int yes = 1;

	sockfd = socket(PF_INET, SOCK_STREAM, 0);

	memset(&my_addr, 0, sizeof my_addr); // zero the rest of the struct
	my_addr.sin_family = AF_INET;	// HOST BYTE ORDER
	my_addr.sin_port = htons(port); // short, network byte order
	my_addr.sin_addr.s_addr = inet_addr(addr); // htonl()
	

		// Sometimes you might notice, "bind()" fails, claiming "Address already in use."
	//  what does that mean ?
	//  Well, sometimes a server socket that was connected is still hanging around in the kernel, and it's hogging the port.
	//  You can either wait for it to clear (a minute or so),
	//  Or add code to your program allowing it to reuse the port: setsockopt() fct with SO_REUSEADDR flags, before binding.
	
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		std::cout << "setsockopt()" << std::endl;
		throw 200;
	}

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		std::cout << "bind()" << std::endl;
		throw 200;
	}
	
	// ==> listen() is non-blocking. accept() blocks and returns a client<>server socket upon connection.

	/* Once the socket has been created and bound to a local address, we can cause it to start listening for connections with the listen() function. */
	if (listen(sockfd, 128) < 0)
	{
		std::cout << "listen()" << std::endl;
		throw 5;
	}

	/* The second argument to "listen()", which is 10 in this case,
	 * tells "listen()" how many connections it is allowed to queue up.
	 * If many clients are connecting to our server all at once, and we aren't dealing with them fast enough.
	 * Then the operating system begins to queue up these incoming connections.
	 * If 10 connections become queued up, then the operating system will reject new connections until we remove one from the existing queue.
	 */


	/* After the socket has begun listening for connections, we can accept any incoming connection with the "accept()" function */
	std::cout << "Waiting for connection " << std::endl;
	
	return (sockfd);
}
