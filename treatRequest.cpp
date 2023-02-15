#include"tools_cpp.hpp"

void generate_autoindex(Client& client, std::string dirPath)
{
    DIR *dir;
    struct dirent *dirp;
    struct stat stats;
    struct tm dt;
    std::fstream fs;

    //std::cout << "dir to show index of is " << dirPath << std::endl;
    if ((dir = opendir(dirPath.c_str())) != NULL)
    {
        fs.open("directoryIndex.html", std::fstream::out);
        fs << "<html><head><title>Index of ";
        fs << client.request.data.request_target;
        fs << "</title>";
        fs << "<style>body{background-color:rgb(242, 180, 56);color:#FFF;display:flex;justify-content:center;align-items:center;flex-direction:column;min-height:96vh;}";
        fs << "pre {display:flex;justify-content:center;align-items:center;flex-direction:column;width:460px;}";
        fs <<  "a {text-decoration:none;color:#FFF;overflow:hidden;text-overflow:ellipsis;}";
        fs <<  ".data span {font-size:12px}";
        fs <<  ".data {display:flex;justify-content:space-between;font-size:15px;background:#234;padding:.5rem;border-radius:.6rem;min-width:35rem;padding-left:2rem;margin:.2rem;}</style>";
        fs << "</head><body><h1>Index of ";
        fs << client.request.data.request_target;
        while ((dirp = readdir(dir)) != NULL)
        {
            if (std::string(dirp->d_name).compare(".") == 0 || std::string(dirp->d_name).compare("directoryIndex.html") == 0)
                continue;
            if (stat((std::string(dirPath).append(dirp->d_name)).c_str(), &stats) == 0)
            {
                dt = *(gmtime(&stats.st_ctime));
                if (std::string(dirp->d_name).compare("..") == 0)
                {
                    fs << "<div class='data'><a title='../' href='"; fs << client.request.data.request_target;
                    if (client.request.data.request_target.back() != '/')
                        fs << "/";
                    fs << "../'>../</a><span></span></div>\n";
                }
                else
                {
                    fs << "<div class='data'><a title='" ; fs << dirp->d_name; fs << "' href='"; fs <<  client.request.data.request_target;
                    if (client.request.data.request_target.back() != '/')
                        fs << "/";
                    fs << dirp->d_name; fs <<  "'>"; fs << dirp->d_name; fs << "</a><span>";
                    fs << ((dt.tm_mday < 10) ? "0" : ""); fs << dt.tm_mday; fs << "/"; fs << ((dt.tm_mon < 10) ? "0" : ""); fs << dt.tm_mon; fs <<  "/"; fs << dt.tm_year + 1900; fs << " ";
                    fs << ((dt.tm_hour < 10) ? "0" : "") ; fs << dt.tm_hour ; fs << ":" ; fs << ((dt.tm_min < 10) ? "0" : "") ; fs << dt.tm_min ; fs << "</span></div>\n";
                }
            }
        }
        closedir(dir);
        fs.close();
        genereate_response(client, 200, "directoryIndex.html", 0, "", 1);
    }
    genereate_response(client, 403, "", 0, "", 0);
}

int method_not_allowed(std::string method, LocationBlock& location)
{
    size_t size = location.allowed_methods.size();
    size_t i = 0;

    while(i != size)
        if (method == location.allowed_methods[i++])
            return 0;
    return 1;
}

int exists_test(Client& client, LocationBlock& location, struct stat* st, std::string& str)
{
    //std::cout << "GET THE EXTENDED PATH fct\n";
    std::string path_loc;
    std::string root;
    std::string url;

    root = location.root;
    path_loc = location.path;
    url= client.request.data.request_target; // URL
    
    if (path_loc.empty())
        str = root.append(url);  // url + root
    else if (path_loc == "/")
        str = root.append(url); // url + root
    else
    {
        size_t i;
        for (i = 0; i < path_loc.length(); i++)
        {
            if (path_loc[i] != url[i])
                break;
        }
        //std::cout << "i = " << i << "\n";
        url = url.substr(i, std::string::npos);
        //std::cout << "new url " << url << std::endl;
        str = root.append(url);
    }
    //std::cout << "NEW TARGET " << str << std::endl;
    if (stat(str.c_str(), st) < 0)
        genereate_response(client, 404, "", 0, "", 0);
    return (S_ISDIR((*st).st_mode));
}

std::string find_index(Client& client, LocationBlock& location, std::string dir)
{
    size_t size;
    size_t i = 0;
    struct stat st;
    std::string str;

    size = location.index.size();
    if (size)
    {
        while(i != size)
        {
            str = std::string(dir).append(location.index[i]);
            //std::cout << "index " << str << std::endl;
            if (stat(str.c_str(), &st) < 0)
                i++;
            else
            {
                if (S_ISDIR(st.st_mode))
                {
                    str = "http://";
                    str.append(search(client.request.data.headers, "host", 0));
                    str.append(client.request.data.request_target);
                    str.append("/");
                    str.append(location.index[i]);
                    str.append("/");
                    genereate_response(client, 301, "", 0, str, 0);
                }
                else if (!(st.st_mode & S_IRUSR))
                    genereate_response(client, 403, "", 0, "", 0);
                return (str);
            }
        }
        return ("");
    }
    str = std::string(dir).append("index.html");
    if (stat(str.c_str(), &st) < 0)
        return ("");
    else
    {
        if (S_ISDIR(st.st_mode))
        {
            str = "http://";
            str.append(search(client.request.data.headers, "host", 0));
            str.append(client.request.data.request_target);
            str.append("/index.html/");
            genereate_response(client, 301, "", 0, str, 0);
        }
        else if (!(st.st_mode & S_IRUSR))
            genereate_response(client, 403, "", 0, "", 0);
        return (str);
    }
    return ("");
}

int cgi_exist(std::string path)
{
    struct stat st;

    if (stat(path.c_str(), &st) < 0 || S_ISDIR(st.st_mode) || !(st.st_mode & S_IXUSR))
        return 0;
    return 1;
}

int findMatchingCgi(Client& client, std::string file, std::string& cgi_path)
{
    std::string extension;
    size_t i;
    std::string ext;

    if ((i =  file.find_last_of(".")) == std::string::npos || i + 1 == file.size())
        return 0;
    extension = file.substr(i + 1, std::string::npos);
    extension = to_uper(extension);
    std::map<std::string, std::string>::iterator it = client.request.data.location.cgi_path.begin();
    std::map<std::string, std::string>::iterator end = client.request.data.location.cgi_path.end();
    while(it != end)
    {
        ext = it->first.substr((it->first.find('_')) + 1, std::string::npos);
        ext = to_uper(ext);
        if (ext == extension)
        {
            if (!cgi_exist(it->second))
                return 0;
            cgi_path = it->second;
            return 1;
        }
        it++;
    }
    return 0;
}

void    directory(Client& client, LocationBlock& location, struct stat &st, std::string target)
{
    std::string indexfile;
    std::string str;
    std::string cgi_path;

    if (target.back() != '/')
        target.append("/");
    if (client.request.data.method == "DELETE")
    {
        if (location.cgi_path.size() != 0)
        {
            if ((str = find_index(client, location, target)) == "" || !findMatchingCgi(client, str, cgi_path))
                genereate_response(client, 403, "", 0, "", 0);
            cgi_function(client, cgi_path, str);//////////////////////////////////////////////////////////////////////////////
        }  
        else if (location.cgi_path.size() == 0)
        {
            if ((st.st_mode & S_IXUSR) && (st.st_mode & S_IWUSR))
            {
                str = "rm -rf ";
                str.append(target);
                str.append("*");
                system(str.c_str());
                genereate_response(client, 204, "", 0, "", 0);
            }
            genereate_response(client, 403, "", 0, "", 0);
        }
    }
    else
    {
        str = find_index(client, location, target);
        if (str == "" && client.request.data.method == "POST")
            genereate_response(client, 403, "", 0, "", 0);
        if (str == "" && client.request.data.method == "GET" && location.autoindex == "off")
            genereate_response(client, 403, "", 0, "", 0);
        if (str == "" && client.request.data.method == "GET" && location.autoindex == "on")
            generate_autoindex(client, target);
        if (str != "" && location.cgi_path.size() && findMatchingCgi(client, str, cgi_path))
            cgi_function(client, cgi_path, str);//////////////////////////////////////////////////////////////////////////////
        if (client.request.data.method == "GET")
            genereate_response(client, 200, str, 0, "", 0);
        if (client.request.data.method == "POST")
            genereate_response(client, 403, "", 0, "", 0);
    }
}

void upload(Client& client, LocationBlock& location)
{
    std::fstream fs;
    std::fstream fstream;
    std::string date;
    std::string type;
    size_t i = 0;

    fstream.open(client.request.data.bodypath, std::fstream::in);
    fstream.seekg(0, std::ios::end);
    if (location.client_max_body_size !=-1 && fstream.tellg() > location.client_max_body_size)
    {
        fstream.close();
        genereate_response(client, 413, "", 0, "", 0);
    }
    date = "/";
    std::time_t now = std::time(NULL);
    date.append(std::ctime(&now));
    date.erase(date.size() - 1, 1);
    while (i != date.size())
    {
        if (date[i] == ' ')
            date[i] = '_';
        i++;
    }
    type = search(client.request.data.headers, "content-type", 0);
    if ((type = search(mimeTypes, type, 1)) != "")
    {
        date.append(".");
        date.append(type);
    }
    fs.open(location.upload.append(date), std::fstream::out);
    if (fs)
    {
        fstream.seekg(0,std::ios_base::beg);
        fs << fstream.rdbuf();
        fs.close();
        fstream.close();
        genereate_response(client, 201, "", 0, date.substr(1, std::string::npos), 0);
    }
    else
    {
        fstream.close();
        genereate_response(client, 403, "", 0, "", 0);
    }
}

void file(Client& client, LocationBlock& location, struct stat& st, std::string file_path)
{
    std::string cgi_path;

    if (location.cgi_path.size() == 0 || (location.cgi_path.size() && !findMatchingCgi(client, file_path, cgi_path)))
    {
        if (client.request.data.method == "GET" && (st.st_mode & S_IRUSR))
            genereate_response(client, 200, file_path, 0, "", 0);
        else if (client.request.data.method == "GET" && !(st.st_mode & S_IRUSR))
            genereate_response(client, 403, "", 0, "", 0);
        if (client.request.data.method == "DELETE")
        {
            remove(file_path.c_str());
            if (stat(file_path.c_str(), &st) < 0)
                genereate_response(client, 204, "", 0, "", 0);
        }
        genereate_response(client, 403, "", 0, "", 0);
    }
    cgi_function(client, cgi_path, file_path);//////////////////////////////////////////////////////////////////////////////
}

void treatRequest(Client& client)
{
    int is_dir;
    struct stat st;
    LocationBlock& location = client.request.data.location;
    std::string right_path;

    try
    {
        if (client.response.error && client.response.error != 411) // in case error of parsing request
            genereate_response(client, client.response.error, "", 1, "", 0);
        else if (client.response.error)
            genereate_response(client, client.response.error, "", 0, "", 0);
        if (client.request.data.method != "GET" && client.request.data.method != "POST" &&client.request.data.method != "DELETE")
            genereate_response(client, 501, "", 0, "", 0);
        if (location.redirection.size() != 0) // redirection in location
            genereate_response(client, std::stoi(location.redirection[0]), "", 0, location.redirection[1], 0);
        if (location.allowed_methods.size() != 0 && method_not_allowed(client.request.data.method, location)) 
            genereate_response(client, 405, "", 0, "", 0);
        if (client.request.data.method == "POST" && location.upload.size() != 0) // Post method and  upload
            upload(client, location);
        is_dir = exists_test(client, location, &st, right_path); // expand path of ressource
        if (is_dir) 
            directory(client, location, st, right_path);
        file(client, location, st, right_path);
    }
    catch (int)
    {
        return ;
    }
}