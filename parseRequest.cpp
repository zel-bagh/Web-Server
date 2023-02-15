#include "tools_cpp.hpp"

std::map<std::string, std::string> mimeTypes;
std::map<std::string, std::string> codeDescription;

void    find_serverblock(Request& r,std::string host, std::vector<ServerBlock>&  blocks)
{
    size_t size;
    size_t i = 0;
    const std::vector<std::string>* server_names;
    std::vector<ServerBlock>::iterator it = blocks.begin();
    std::vector<ServerBlock>::iterator end = blocks.end();

    //std::cout << "Find serverblock fct\n";
    
    /*std::vector<std::string> tokens;
    tokens = split_token_with_del(host, ':'); // separtor
    host = tokens[0];
    // int port = atoi(tokens[1].c_str());*/
    size_t found;
    found = host.rfind(":");
    if (found != std::string::npos)
    {
        host = host.substr(0, found);
    }
    while(it != end)
    {
        server_names = &(it->server_names);
        size = server_names->size();
        i = 0;
        while(i != size)
        {
           // std::cout << "server name: " <<(*server_names)[i] << " | host:" << host << std::endl;
            if ((*server_names)[i] == host)
            {
                //std::cout << "---->" << (*server_names)[i] << " " << host << std::endl;
                r.data.location = find_location(*it, r.data.request_target);
                r.data.server = &(*it);
                //std::cout << "----->" << r.data.location.root << std::endl;
                return ;
            }
            i++;
        }
        it++;
    }
    r.data.location = find_location(blocks[0], r.data.request_target);
    r.data.server = &(blocks[0]);
}

void    method(std::string m, std::string &method)
{
    int size;

    size = m.size();
    if (size == 0)
        throw 400;
    method = m;
}

void    request_target(std::string rt, std::string &request_target, std::string &query)
{
    size_t size;
    size_t i = 0;
    int idx = -1;

    std::string allowed= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
    size = rt.size();
    if (size == 0 || rt[0] != '/')
        throw 400;
    while (i != size)
    {
        if (allowed.find(rt[i]) == std::string::npos)
            throw 400;
        if (idx == -1 && rt[i] == '?')
            idx = i;
        i++;
    }
    if (idx == -1)
        request_target = rt;
    else
    {
        request_target = rt.substr(0, idx);
        query = rt.substr(idx, std::string::npos);
    }
}

void    http_version(std::string h)
{
    size_t size;
    size_t i;
    int point_count;

    size = h.size();
    if (size < 6 || h.substr(0, 5) != "HTTP/" || h[5] == '0' || h[5] == '.' || h[size - 1] == '.')
        throw 400;
    i = 4;
    point_count = 0;
    while (++i != size)
    {
        if ((h[i] < 48 || h[i] > 57) && h[i] != '.')
           throw 400;
        if (h[i] == '.')
            point_count++;
        if (point_count > 1)
         throw 400;
    }
    if (h.substr(5, std::string::npos) != "1.1")
        throw 505;
}

void request_line(std::string rl, request_data &r)
{
    int c;
    size_t idx;

    if (rl.size() == 0)
        throw 400;
    c = 0;
    for(;;)
    {
        if (rl.size() == 0)
            break ;
        c++;
        if (c != 3 && (idx = rl.find(" ")) == std::string::npos)
            throw 400;
        else if (c == 3 && rl.find(" ") != std::string:: npos)
            throw 400;
        if (c == 1)
            method(rl.substr(0, idx), r.method);
        else if (c == 2)
            request_target(rl.substr(0, idx), r.request_target, r.query);
        else if (c == 3)
            http_version(rl.substr(0, std::string::npos));
        if (c != 3)
            rl.erase(0, idx + 1);
        else
            rl.erase(0, std::string::npos);
    }
    if (c != 3)
        throw 400;
}

unsigned long long ft_atoi(std::string value)
{
    unsigned long long result;
    int i;
    int size;

    size = value.size();
    result = 0;
    i = -1;
    while (++i != size)
        result = (result * 10) + (value[i] - 48);
    return result;
}
void    valid_content_length(std::string value, unsigned long long int &content_length)
{
    int i;
    int size;

    i = -1;
    size = value.size();
    while (++i != size)
        if (value[i] < 48 || value[i] > 57)
            throw 400;
    i = -1;
    while (++i !=  size)
        if (value[i] != 48)
            break ;
    if (i == size)
        content_length = 0;
    else if (i != size)
    {
        value.erase(0, i);
        if (size > 20)
            throw 400;
        else if (size == 20 && value.compare("18446744073709551615") == 1)
            throw 400;
        content_length = ft_atoi(value);
    }
}

int    known_transfer_encoding(std::string tec)
{
    int i;

    if (tec.size() == 0)
        throw 400;
    std::string known_tec[1] = {"chunked"};
    i = -1;
    while (++i != 1)
        if (tec == known_tec[i])
        {
            if (tec == "chunked")
                return (1);
            return (0);
        }
    throw 501;
}

void    valid_transfer_encoding(std::string value)
{
    size_t i;
    std::string tec;
    int chunked_count;

    chunked_count = 0;
    while (1)
    {
        i = 0;
        while (i != value.size() && value[i] != ',')
            i++;
        tec = value.substr(0, i);
        chunked_count += known_transfer_encoding(tec);
        if (i == value.size())
            break ;
        else
            value.erase(0, i + 1);
    }
    if (chunked_count != 1)
        throw 400;
}

void    value_matters_at_this_point_headers(std::string name, std::string value, unsigned long long int &content_length)
{
    int i;

    std::string header_names[3] = {"host", "content-length", "transfer-encoding"};
    i = -1;
    while(++i != 3)
        if (name == header_names[i])
        {
            if (value.size() == 0 && name == "content-length")
                throw 411;
            else if (value.size() == 0)
                throw 400;
            if (name == "content-length")
                valid_content_length(value, content_length);
            else if (name == "transfer-encoding")
                valid_transfer_encoding(value);    
            break ;
        }
}

void header_format(std::string &h, std::string &header_name, std::string &header_value)
{
    int idx;
    size_t size;
    size_t i;

    i = 0;
    idx = 0;
    size = h.size();
    while(i != size && h[i] != ':')
    {
        if (h[i] == ' ')
            throw 400;
        idx = 1;
        i++;
    }
    if (idx == 0)
        throw 400;
    header_name = h.substr(0,i);
    if (h[i] == ':' && i != size - 1)
    {
        while (++i != size)
            if (h[i] != ' ')
                break ;
        if (i != size)
        {
            header_value = h.substr(i, std::string::npos);
            i = header_value.size() - 1;
            while (header_value[i] == ' ')
                i--;
            if (i != header_value.size() - 1)
                header_value.erase(i + 1, std::string::npos);
        }
    }
}

void    store_headers(std::string name, std::string value, std::map<std::string, std::string> &headers)
{
    if (headers.size() != 0)
    {
        std::map<std::string, std::string>::iterator it = headers.begin();
        std::map<std::string, std::string>::iterator ite = headers.end();
        while (it != ite)
        {
            if (it->first == name)
            {
                if (name == "connection" || name == "host" || name == "content-length")
                    throw 400;
                it->second = it->second.append(",");
                it->second = it->second.append(value);
                if (name == "transfer-encoding")
                    valid_transfer_encoding(it->second);
                return ;
            }
            it++;
        }
    }
    headers.insert(std::pair<std::string, std::string>(name, value));
}

void    header_field(std::string h, int& cl_exist, int& tr_exist, int& host_exist, Request &r, std::vector<ServerBlock> &blocks)
{
    size_t i;
    size_t size;
    std::string header_name;
    std::string header_value;

    i = 0;
    header_format(h, header_name, header_value);
    size = header_name.size();
    while (i != size)
    {
        if (header_name[i] >= 65 && header_name[i] <= 90)
            header_name[i] = header_name[i] + 32;
        i++;
    }
    if (header_name == "connection" || header_name == "transfer-encoding" || header_name == "content-type")
    {
        i = 0;
        while (i != size)
        {
            if (header_value[i] >= 65 && header_value[i] <= 90)
                header_value[i] = header_value[i] + 32;
            i++;
        }
    }
    if (header_name == "content-length")
        cl_exist = 1;
    else if (header_name == "transfer-encoding")
    {
        tr_exist = 1;
        r.transfer_encoding = 1;
    }
    value_matters_at_this_point_headers(header_name, header_value, r.content_length);
    if (header_name == "host")
    {
        host_exist = 1;
        find_serverblock(r,header_value, blocks);
    }
    store_headers(header_name, header_value, r.data.headers);
}

void header_fields(std::string h, Request &r, std::vector<ServerBlock> &blocks)
{
    int idx;
    int host_exist;
    int cl_exist;
    int tr_exist;

    if (h.size() == 0)
        throw 400;
    host_exist = 0;
    cl_exist = 0;
    tr_exist = 0;
    for(;;)
    {
        idx = h.find("\r\n");
        header_field(h.substr(0, idx), cl_exist, tr_exist, host_exist, r, blocks);
        h.erase(0, idx + 2);
        if (h.size() == 0)
            break ;
    };
    if(cl_exist && tr_exist)
        throw 400;
    if (host_exist == 0)
        throw 400;
}

void    print_headers(std::map<std::string, std::string> &headers)
{
    std::cout << "====headers===="<< std::endl;
        std::map<std::string, std::string>::iterator it = headers.begin();
        std::map<std::string, std::string>::iterator ite = headers.end();

        while (it != ite)
        {
            std::cout << it->first << ": " << it->second << std::endl;
            it++;
        }
    std::cout << "==============" << std::endl;
}
void    print_packet(Client &packet)
{
    size_t i = 0;

    std::cout << std::endl;
    std::cout << "============REQUEST============="<< std::endl; 
    while (i != packet.request.frequest.size())
    {
        if (packet.request.frequest[i] == '\r')
            std::cout << "\\r";
        else if (packet.request.frequest[i] == '\n')
            std::cout << "\\n\n";
        else
            std::cout << packet.request.frequest[i];
        i++;
    }
    std::cout << "================================" << std::endl;
    std::cout << "method:" << packet.request.data.method << std::endl;
    std::cout << "request_target:" << packet.request.data.request_target << std::endl;
    print_headers(packet.request.data.headers);
    std::cout << "content:" << packet.request.content << std::endl;
    std::cout << "error:" << packet.response.error << std::endl;
    std::cout << "response_length:" << packet.response.streamLength<< std::endl;
    std::cout << "body_path:" << packet.request.data.bodypath<< std::endl;
}

void read_request(Client &packet, std::vector<ServerBlock>&  blocks)
{
    int index;

    try
    {
        index = packet.request.frequest.find("\r\n");
        request_line(packet.request.frequest.substr(0,index), packet.request.data);
        header_fields(packet.request.frequest.substr(index + 2, std::string::npos), packet.request, blocks);
        if (packet.request.data.method == "POST" && packet.request.content_length == 0
            && packet.request.transfer_encoding == 0)
            throw 411;
    }
    catch(int status_code)
    {
        packet.response.error = status_code;
        throw 1;
    }
    if (packet.request.transfer_encoding == 0 && packet.request.content_length == 0)
        throw 1;
}

void search_for_request(Client &packet, std::vector<ServerBlock>&  blocks)
{
    size_t index;
    std::fstream fs;
    std::string file;

    if (packet.request.data.method.size() == 0)
    {
        if ((index = packet.request.content.find("\r\n\r\n")) == std::string::npos)
        {
            packet.request.frequest.append(packet.request.content);
            packet.request.content.clear();
            throw 0;
        }
        packet.request.frequest.append(packet.request.content.substr(0, index + 2));
        packet.request.content.erase(0, index + 4);
        read_request(packet, blocks);
        throw 2;
    }
    else
    {
        if (packet.request.data.method != "POST")
        {
            if (packet.request.transfer_encoding == 0)
            {
                if (packet.request.content.size() >= packet.request.content_length)
                {
                    packet.request.content.erase(0, packet.request.content_length);
                    throw 1;
                }
                else
                {
                    packet.request.content_length -= packet.request.content.size();
                    packet.request.content.clear();
                    throw 0;
                }
            }
            else
                chunk_body(packet, fs, 0);
        }
        else
        {
            file = std::to_string(packet.getSockid());
            packet.request.data.bodypath = file.append("_body.tmp");
            fs.open(packet.request.data.bodypath, std::fstream::out | std::fstream::app);
            if (packet.request.transfer_encoding == 0)
            {
                if (packet.request.content.size() >= packet.request.content_length)
                {
                    fs << packet.request.content.substr(0, packet.request.content_length);
                    packet.request.content.erase(0, packet.request.content_length);
                    fs.close();
                    throw 1;
                }
                else
                {
                    fs << packet.request.content;
                    packet.request.content_length -= packet.request.content.size();
                    packet.request.content.clear();
                    fs.close();
                    throw 0;
                }
            }
            else
                chunk_body(packet, fs, 1);
        }
    }
}

int parseRequest(Client &packet, std::vector<ServerBlock>& blocks)
{
    for(;;)
    {
        try
        {
            search_for_request(packet, blocks);
        }
        catch(int z)
        {
            if (z == 0)
                return 0;
            if (z == 1)
                return (1);
        }
    }
}