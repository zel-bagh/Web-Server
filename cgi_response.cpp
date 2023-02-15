#include"tools_cpp.hpp"

void content_length(Client&client, std::string& file)
{
	std::fstream fs;
	fs.open(file, std::fstream::in);
	fs.seekg(0, std::ios::end);
	client.response.stream << "Content-length: " << fs.tellg() << "\r\n";
	fs.close();
}
void	connection(Client& client)
{
	if (search(client.request.data.headers, "connection" , 0) == "close")
    {
        client.response.stream << "connection: close\r\n\r\n";
        client.response.connection = 0;
    }
    else
    {
        client.response.stream << "connection: keep-alive\r\n\r\n";
        client.response.connection = 1;
    }
}
void	body(Client& client, std::string file)
{
	std::fstream fs;
	fs.open(file, std::fstream::in);
	fs.seekg(0, std::ios::end);
	if (fs.tellg() == 0)
		fs.close();
	else
	{
		fs.seekg(0, std::ios::beg);
		client.response.stream << fs.rdbuf();
		fs.close();
	}
	remove(file.c_str());
}

void cgi_response(Client& client,int status_code, std::string file, std::map<std::string, std::string> cgi_headers)
{
	std::string code = std::to_string(status_code);
    client.response.stream << "HTTP/1.1 " << code << " " << search(codeDescription, code, 0) << "\r\nServer: Emy\r\n";
	std::map<std::string, std::string>::iterator it = cgi_headers.begin();
	std::map<std::string, std::string>::iterator end = cgi_headers.end();
	while(it != end)
	{
		client.response.stream << it->first << ":" << it->second;
		it++;
	}
	content_length(client, file);
	connection(client);
	body(client, file);
	client.response.streamLength = client.response.stream.tellp();
	client.response.stream.seekg(0, std::ios::beg);
	if (client.request.data.bodypath.size())
        remove(client.request.data.bodypath.c_str());
	throw 0;
}