#include "tools_cpp.hpp"

std::string search(std::map<std::string, std::string>& map, std:: string str, int i)
{
    std::map<std::string, std::string>::iterator it = map.begin();
    std::map<std::string, std::string>::iterator end = map.end();

    while (it != end)
    {
        if (i == 1)
        {
            if (it->second.compare(str) == 0)
                return it->first;
        }
        else if(i == 0)
        {
            if (it->first.compare(str) == 0)
            {
                return it->second;
            }
        }
        it++;
    }
    return ("");
}

std::string file_type(std::string& file)
{
    size_t i;
    std::string str;

    if ((i = file.find_last_of(".")) == std::string::npos)
        return "application/octet-stream";
    str = search(mimeTypes, file.substr(++i), 0);
    return (str == "") ? "application/octet-stream" : str;
}

int getfile(Client& client, std::string& status_code, std::string& file)
{
    std::map<int, std::string>::iterator it;
    std::map<int, std::string>::iterator end;
    // std::cout << "getfile function\n";
    it = client.request.data.location.Error_page.begin();
    end = client.request.data.location.Error_page.end();
    while (it != end)
    {
        if (it->first == std::stoi(status_code))
        { 
            file = it->second;
            return 1;
        }
        it++;
    }
    return 0;
}

void content_length(Client &client, std::string& status_code, std::string& file, std::fstream& fs, std::string& redloc)
{
    unsigned long long int length = 0;
    

    if (status_code[0] == '4' || status_code[0] == '5' || status_code == "301" || status_code == "302" || status_code == "303")
    {
        length = 2;
        if (client.request.data.location.Error_page.size() != 0)
            if (getfile(client, status_code, file) == 1)
                length = 1;
    }
    else if (status_code == "305")
        length = 3;
    else if (status_code == "304" || file.size() == 0)
        return ;
    if (length != 2 && length != 3)
    {
        fs.open(file, std::fstream::in);
        if (fs)
        {
            fs.seekg(0, std::ios::end);
            length = fs.tellg();
            // std::cout << "length = " << length<< std::endl;
        }
        else
            length = 126 + (2 * search(codeDescription, status_code, 0).size());
    }
    else if (length == 2)
        length =  126 + (2 * search(codeDescription, status_code, 0).size());
    else if (length == 3)
        length = redloc.size();
    client.response.stream << "content-length: " << length << "\r\n";
}

void location(Client& client, std::string& status_code, std::string& redloc)
{
    if (status_code == "301" || status_code == "302" || status_code == "303" || status_code == "201")
        client.response.stream << "location: " << redloc << "\r\n";
}
void content_type(Client& client, std::string& status_code, std::fstream& fs, std:: string& file)
{
    std::string type;

    if (status_code == "304")
        return ;
    else if (status_code == "305")
        type = "application/octet-stream";
    else if (fs.is_open())
        type = file_type(file);//;
    else if (status_code[0] != '2')
        type = "text/html";
    else
        return ;
    client.response.stream << "content-type: " << type << "\r\n";
}

void connection(Client& client, int close)
{
    if (close || search(client.request.data.headers, "connection" , 0) == "close")
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

void body(Client& client, std::string status_code, std::fstream& fs, std::string& redloc)
{
    if (status_code == "304")
        return ;
    else if (status_code == "305")
        client.response.stream << redloc;
    else if (fs.is_open())
    {
        fs.seekg(0, std::ios::end);
        if (fs.tellg() == 0)
		    fs.close();
        else
        {
        fs.seekg(0, std::ios::beg);
        client.response.stream << fs.rdbuf();
        fs.close();
        }
    }
    else if (status_code[0] != '2')
    {
        client.response.stream << "<html>\n<head><title>";
        client.response.stream << status_code;
        client.response.stream << " ";
        client.response.stream << search(codeDescription, status_code, 0);
        client.response.stream << "</title></head>\n<body>\n<center><h1>";
        client.response.stream << status_code;
        client.response.stream << " ";
        client.response.stream << search(codeDescription, status_code, 0);
        client.response.stream << "</h1></center>\n<hr><center>Emy server</center>\n</body>\n</html>\n";
    }
}

void genereate_response(Client& client, int status_code, std::string file, int close, std::string redloc, int deletFile)
{
    std::fstream fs;
    std::string code = std::to_string(status_code);
    client.response.stream << "HTTP/1.1 " << code << " " << search(codeDescription, code, 0) << "\r\nServer: Emy\r\n";
    content_length(client, code, file, fs, redloc);
    content_type(client, code, fs, file);
    location(client, code, redloc);
    connection(client, close);
    body(client, code, fs, redloc);
    if (file.size() && deletFile)
        remove(file.c_str());
    client.response.streamLength = client.response.stream.tellp();
    //std::cout << "response length: " << client.response.streamLength << std::endl;
    if (client.request.data.bodypath.size())
        remove(client.request.data.bodypath.c_str());
    client.response.stream.seekg(0, std::ios::beg);
    throw 0;
}