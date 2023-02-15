# ifndef TOOLS_CPP_HPP
#define TOOLS_CPP_HPP

#include "File_hpp/Client.hpp"
#include "File_hpp/ServerBlock.hpp"
#include "File_hpp/ListenDirective.hpp"
#include "File_hpp/Request.hpp"
#include "File_hpp/Response.hpp"
#include "File_hpp/LocationBlock.hpp"


extern std::map<std::string, std::string> mimeTypes;
extern std::map<std::string, std::string> codeDescription;

int	parsingConfigFile(char *argv, std::vector<ServerBlock>& servers);
void GetListenerServers(std::vector<ServerBlock>& servers, std::map<ListenDirective, std::vector<ServerBlock> >& listeners);
void printServer(ServerBlock& server);

std::vector<std::string> split_line_to_tokens(std::string line);
std::vector<std::string> split_token_with_del(std::string line, int del);

int checkForAcoladeServ(std::vector<std::string>& tokens, std::ifstream& conf_file);
int checkForAcoladeLocation(std::vector<std::string>& tokens, LocationBlock& newServer, std::ifstream& conf_file);
int     fill_server(std::vector<std::string> tokens,ServerBlock& newServer);
int    fill_location(std::vector<std::string> tokens, LocationBlock& newLocation);
int find_del(std::string str, int del);

int parseServer(ServerBlock& newServer, std::vector<std::string>& tokens, std::ifstream& config_file);
int parseLocation(LocationBlock& newLocation, std::vector<std::string>& tokens, std::ifstream& config_file);


int checkSemicolon(std::vector<std::string>& tokens);
int checkItsNumber(std::string str);
int checkListenDirective(std::vector<std::string>& tokens, ServerBlock& newServer);
void	networkProgramming(std::vector<ServerBlock>& servers);
int	createSock(int port, const char* addr);
void	readFromSock(std::map<ListenDirective, std::vector<ServerBlock> > map_listeners, std::vector<Client*>& Clients, fd_set* WriteSet, fd_set* readyToRead, fd_set *ReadSet);
void writeToSock(std::vector<Client*>& Clients, fd_set* WriteSet, fd_set* readyToWrite, fd_set *ReadSet);
int sendResponse(int sockid, Response& response);


int parseRequest(Client& packet, std::vector<ServerBlock>&  blocks);
void chunk_body(Client &packet, std::fstream& fs, int store);
void store_mimeTypes(void);
void store_codeDescription(void);
//void genereate_response(Client& client, int status_code, std::string file, int close, std::string redloc, int deletFile);
void treatRequest(Client& client);
//std::string search(std::map<std::string, std::string>& map, std:: string str, int i);
int is_string_number(std::string& str);
LocationBlock    find_location(ServerBlock& server, std::string target);

void cgi_function(Client& client, std::string cgiPath, std::string filePath);
std::string search(std::map<std::string, std::string>& map, std:: string str, int i);
std::string to_uper(std::string str);
void genereate_response(Client& client, int status_code, std::string file, int close, std::string redloc, int deletFile);
void cgi_response(Client& client,int status_code, std::string file, std::map<std::string, std::string> cgi_headers);

size_t	ft_time_seconds(void);
#endif
