#include "tools_cpp.hpp"

void initializeClientData(Client* client)
{
	std::string file;

	file = std::to_string(client->getSockid()) + "_response";
    //std::cout << "file :---------------------------------------------------------------------" << file << "\n";
    
	client->response.stream.open(file, std::ios::in | std::ios::out | std::ios::trunc);
    
	// if (client->response.stream.is_open())
    // {
    //    	if (client->response.stream.tellg() == 0)
	// 	{
	// 		std::cout << "NIIIIIICE initaileise stream" << std::endl;
	// 	} 
    // }
    // else
    //     std::cout << "New stream can't open\n";

	/*Request tmp(client->getSockid());
	tmp.content = client->request.content;
	client->request = tmp;*/

    client->request.frequest.clear();
	//std::cout << " client->request.frequest  " << client->request.frequest << std::endl;
    client->request.data.bodypath.clear();
	//std::cout << "client->request.data.bodypath  " << client->request.data.bodypath<< std::endl;
	client->request.data.headers.clear();
	//std::cout << "client->request.data.headers " << client->request.data.headers.begin()->first << " | " <<  client->request.data.headers.begin()->second << std::endl;
	client->request.data.method.clear();
	//std::cout << "client->request.data.method  " << client->request.data.method<< std::endl;
	client->request.data.query.clear();
	//std::cout << "client->request.data.query  " <<client->request.data.query << std::endl;
	client->request.data.request_target.clear();
	//std::cout << "client->request.data.request_target  " << client->request.data.request_target << std::endl;


	 client->request.chk.last_chunk = 0;
	// std::cout << "client->request.chk.last_chunk  " << client->request.chk.last_chunk<< std::endl;
	 client->request.chk.line = 1;
	//std::cout << "client->request.chk.line  " << client->request.chk.line<< std::endl;
	 client->request.chk.line_content.clear();
	//std::cout << " client->request.chk.line_content  " << client->request.chk.line_content << std::endl;

    client->request.content_length = 0;
	//std::cout << " client->request.content_length  " <<  client->request.content_length<< std::endl;
    client->request.transfer_encoding = 0; 
	//std::cout << "client->request.transfer_encoding  " <<client->request.transfer_encoding << std::endl;

	client->response.error = 0;
	client->response.stream_index = 0;
	client->response.streamLength = 0;
	client->response.buf_index = -1;
	client->response.bufLength = 0;
	//std::cout << "--------------------------------------------------------------Inialia done \n";
}

void writeToSock(std::vector<Client*>& Clients, fd_set* WriteSet, fd_set* readyToWrite, fd_set *ReadSet)
{
    int res;
	//std::cout << "ENTER TO CLOOOOOSE CONNECTION\n";


	std::vector<Client*>::iterator it_client;
	//std::cout << "IN WRITE SOCKETS fct \n";
	if (Clients.size() > 0)
	{
    	for(it_client = Clients.begin(); it_client != Clients.end(); it_client++)
		{

			//std::cout << "clients not empty\n";
			if (FD_ISSET((*it_client)->getSockid(), readyToWrite))  // segfault
			{
				res = sendResponse((*it_client)->getSockid(), (*it_client)->response);
				//std::cout << "FD_ISSET CLIENT exist " << res << std::endl;
				if (res == 2)
				{

					FD_CLR((*it_client)->getSockid(), WriteSet);
					FD_CLR((*it_client)->getSockid(), readyToWrite);

					//std::cout << "ERROR Sending in write to socket\n";
					// remove the client
					close((*it_client)->getSockid());
					delete (*it_client);   // new
					Clients.erase(it_client);
					writeToSock(Clients, WriteSet, readyToWrite, ReadSet);  // recursive, solve the segfault
					break ;
				}
				else if (res == 1)
				{
					//std::cout << "Type of connection " << (*it_client)->response.connection << std::endl;
	    			(*it_client)->response.stream.close();
					FD_CLR((*it_client)->getSockid(), WriteSet);
					FD_CLR((*it_client)->getSockid(), readyToWrite);
					if ((*it_client)->response.connection == 1) // 1 == keep connection alive
					{
						//std::cout << "Keep alive\n";
						remove(std::string(std::to_string((*it_client)->getSockid())).append("_response").c_str());
						initializeClientData(((*it_client))); // ptr
						(*it_client)->beginning_time = ft_time_seconds();
						FD_SET((*it_client)->getSockid(), ReadSet);
					}
					else	// close connection
					{
						remove(std::string(std::to_string((*it_client)->getSockid())).append("_response").c_str());
						//std::cout << "CLOOOOOSE CONNECTION\n";
						close((*it_client)->getSockid());
						//std::cout << "before erase clients\n";
						delete (*it_client);   // new
						Clients.erase(it_client);
						writeToSock(Clients, WriteSet, readyToWrite, ReadSet);  // recursive, solve the segfault error
						break ;
					}
				}
			}
		}
	}

}