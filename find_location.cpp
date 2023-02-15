#include "tools_cpp.hpp"

int getLocations(LocationBlock& location, ServerBlock server, std::string& target, std::map<int, std::string> Error_pages)
{
    std::size_t found;
    std::vector<LocationBlock>::iterator iter;

    for (iter = server.locationBlock.begin(); iter != server.locationBlock.end(); ++iter)
    {
        found = (*iter).path.rfind('/');

        if (found == ((*iter).path.length() - 1) && (*iter).path.length() > 1) // if you find '/' in the end of location path remove it
            (*iter).path = (*iter).path.substr(0, (*iter).path.length() - 1);
        //std::cout << "LOCATIONS PATH " << (*iter).path <<  " And target "<< target << "\n";
        if ((*iter).path == target)
        {
            location = (*iter);
            //std::cout << "l999999ayt location\n";
            //std::cout << "location rooooot " << location.root << "\n";

            // Overwrite the Error pages of server block
            std::map<int, std::string>::iterator it;
            for (it = location.Error_page.begin(); it != location.Error_page.end(); ++it)
		        Error_pages[it->first] = it->second;

            location.Error_page = Error_pages;  // changing in the copy

            // Printing location.Error_page
            /*for (it = location.Error_page.begin(); it != location.Error_page.end(); ++it)
	        {
		        std::cout << it->first << " " << it->second << std::endl;
	        }*/
            return (0);
        }
    }
    return (1);
}

int checkLocations(LocationBlock& location, ServerBlock& server, std::string& target, std::map<int, std::string> Error_pages)
{
    std::size_t found;

    found = target.find_last_of('/');
    //std::cout << "cursrue FOUND " << found <<  "on target "<< target << "\n";
    //std::cout << "CHECKLOCATION after\n";
    if (found == std::string::npos && target.find('/') == std::string::npos) // GET / HTTP1.1
    {
        //std::cout << "L9AYT RI SERVER BLOCK\n";
        // return root location 
        location.root = server.root;
        location.autoindex = server.autoindex;
        location.Error_page = Error_pages;
        location.index = server.index;
        return (0);
    }
    else
    {
        //std::cout << "target " << target << "\n";
        if (target[target.length() - 1] == '/' && target.length() > 1)  // GET /images/books/
            target = target.substr(0, target.length() - 1);  // ==> GET /images/books
        //std::cout << "target " << target << "\n";
        //comparing with all locations failed
        if (getLocations(location, server, target, Error_pages))  // comparing with all locations, == 1 error
        {
            if (target[target.length() - 1] == '/' && target.length() == 1)  // case of location "/" does not exist 
            {
                //std::cout << "L9AYT RI SERVER BLOCK\n";
                // return root location 
                location.root = server.root;
                location.autoindex = server.autoindex;
                location.Error_page = Error_pages;
                location.index = server.index;
                return (0);
            }
            if (target[target.length() - 1] == '/' && target.length() > 1)  // in case of failure I'm gonna remove the slash
                target = target.substr(0, target.length() - 1);
            target = target.substr(0, target.length() - (target.length() - found) + 1);  // you can replace it by: found + 1
            checkLocations(location, server, target, Error_pages);  // recursive
        }
    }
    return (0);
}

LocationBlock    find_location(ServerBlock& server, std::string target) // 
{
    LocationBlock   location;
    std::string path;

    //std::cout << "Target " << target << std::endl;

    path = target;

    // ERROR pages

    checkLocations(location, server, path, server.Error_page);

    // PRINT the server Error_page
    // std::map<int, std::string>::iterator it;
    // for (it = server.Error_page.begin(); it != server.Error_page.end(); ++it)
	// 	std::cout << it->first << " " << it->second << std::endl;

    if (location.autoindex.empty())
        location.autoindex = "off";
    if (location.root.empty())
    {
        if (server.root.empty())
            location.root = "/Users/isghioua/Desktop/webserv/fanadesh-html";
        else
            location.root = server.root;
    }
    if (location.allowed_methods.empty())
        location.allowed_methods = server.allowed_methods;
    if (location.client_max_body_size == -1)
        location.client_max_body_size = server.client_max_body_size;
   // exit(1);
    return (location);
    //target = "";
    //return server.locationBlock[0];
}