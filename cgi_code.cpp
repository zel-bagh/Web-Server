#include"tools_cpp.hpp"

std::string to_uper(std::string str)
{
    size_t i = 0;
    size_t size = str.size();

    while(i != size)
    {
        if (str[i] >= 'a' && str[i] <= 'z')
            str[i] -= 32;
        i++;
    }
    return str;
}

void    change_(std::string& str)
{
    size_t i = 0;
    size_t size = str.size();

    while(i != size)
    {
        if (str[i] == '-')
            str[i] = '_';
        i++;
    }
}

void    fill_env(Client& client, char** env, std::string file_path)
{
    int i = -1;
    std::string str;
    env[++i] = strdup("SERVER_SOFTWARE=EMY/1.0");
    env[++i] = strdup("REDIRECT_STATUS=1");
    env[++i] = strdup(std::string("SCRIPT_FILENAME=").append(file_path).c_str());
    if (!client.request.data.server->server_names.empty())
        env[++i] = strdup(std::string("SERVER_NAME=").append(client.request.data.server->server_names[0]).c_str());
    else
        env[++i] = strdup(std::string("SERVER_NAME=").c_str());
    env[++i] = strdup("SERVER_PROTOCOL=HTTP/1.1");
    env[++i] = strdup(std::string("SERVER_PORT=").append(std::to_string(client.request.data.server->listen.port).c_str()).c_str());
    env[++i] = strdup(std::string("REQUEST_METHOD=").append(client.request.data.method).c_str());
    env[++i] = strdup(std::string("QUERY_STRING=").append(client.request.data.query).c_str());
    env[++i] = strdup(std::string("CONTENT_TYPE=").append(search(client.request.data.headers, "content-type", 0)).c_str());
    if (client.request.data.method == "POST")
        env[++i] = strdup(std::string("CONTENT_LENGTH=").append(search(client.request.data.headers, "content-length", 0)).c_str());
    else
        env[++i] = strdup(std::string("CONTENT_LENGTH= 0").c_str());

    std::map<std::string,std::string>::iterator it = client.request.data.headers.begin();
    std::map<std::string,std::string>::iterator end = client.request.data.headers.end();

    while(it != end)
    {
        if ((*it).first == "accept-encoding" || (*it).first == "accept")
            it++;
        else
        {
            str = "HTTP_";
            str.append(std::string(to_uper((*it).first)).append("="));
            change_(str);
            env[++i] = strdup(str.append((*it).second).c_str());
            it++;    
        }

    }
    env[++i] = NULL;
}

void    read_cgi_result(std::map<std::string,std::string>& cgi_headers, int& status_code)
{
    std::fstream fs;
    std::string str;
    int line_count = 0;
    std::string tmp;

    fs.open("cgi_result.txt", std::fstream::in);
    while(1)
    {
        getline(fs, str, '\n');
        line_count++;
        if (str == "\r")
            break;
        if (str.substr(0, str.find(":")) == "Status")
            status_code = std::stoi(str.substr(8, 3));
        else
            cgi_headers.insert(std::pair<std::string, std::string>(str.substr(0, str.find(":")), (str.substr(str.find(":") + 1, std::string::npos)).append("\n")));
    }
    fs.close();
    tmp = std::string("sed '1,").append(std::to_string(line_count + 1));
    tmp.append("d' cgi_result.txt > cgi_result1");
    system(tmp.c_str());
    remove("cgi_result.txt");
    if (status_code == 0)
        status_code = 200;

}

void cgi_function(Client& client, std::string cgiPath, std::string filePath)
{
    char **env;
    pid_t pid;
    int fd;
    int status;
    char *cmd[3];
    int status_code = 0;
    std::string type;
    std::map<std::string,std::string> cgi_headers;

    pid = fork();
    if (pid == 0)
    {
        if (client.request.data.bodypath.size())
        {
            //std::cout << "==================================================body exist\n";
            fd = open(client.request.data.bodypath.c_str(), O_RDONLY);
            dup2(fd, 0);
            close(fd);
        }
        env = NULL;
        if ((env = (char**)malloc((client.request.data.headers.size() + 11) * sizeof(char*))) == NULL)
            exit(1);
        fill_env(client, env, filePath);
        fd = open("cgi_result.txt", O_CREAT|O_WRONLY, 0777);
        dup2(fd, 1);
        close(fd);
       // write(2,"filling env done\n", 17);
        cmd[0] = strdup(cgiPath.c_str());
        cmd[1] = strdup(filePath.c_str());
        cmd[2] = NULL;
        //write(2, "calling execve\n", 15);
        execve(cmd[0], cmd, env);
        exit(1);
    }
    wait(&status);
    if (status != 0)
        genereate_response(client, 500, "", 0, "", 0);
    type = filePath.substr(filePath.find_last_of(".") + 1, std::string::npos);
    type = to_uper(type);
    if (type == "PHP")
    {
        read_cgi_result(cgi_headers, status_code);
        cgi_response(client, status_code, "cgi_result1", cgi_headers);
    }
    genereate_response(client, 200, "cgi_result.txt", 0, "", 1);
}