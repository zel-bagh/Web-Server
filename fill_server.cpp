#include "tools_cpp.hpp"

int    fill_server(std::vector<std::string> tokens,ServerBlock& newServer)
{
    if (checkSemicolon(tokens))
        return(1);

    // std::cout << tokens.at(0) << std::endl;

    if(tokens.at(0).compare("listen")  == 0 && newServer.listen.it_exist == false)
    {  
        // std::cout << "IT'S LISTEN ";
        // can we have more than 1 listen derictive?;// NOPE our webserv don't support more then one directive of listen in a server,  not like nginx: 80 && 443
        if (checkListenDirective(tokens, newServer))   // error = 1
            return (1);
    }
    else if(tokens.at(0).compare("server_name") == 0 && newServer.server_names.empty())
    {
        // std::cout << "IT'S server_name ";
        if (tokens.size() > 1)
        {
            tokens.erase(tokens.begin());
            newServer.server_names = tokens;
        }
        else
            return (1);
    }
    else if(tokens.at(0).compare("root")  == 0 && newServer.root.empty())   // Check for Absolute Path
    {
        // std::cout << "IT'S root ";
        if(tokens.size() == 2 && tokens.at(1)[0] == '/')
             newServer.root = tokens.at(1);
        else
            return(1);
    }
    else if(tokens.at(0).compare("index")  == 0 && newServer.index.empty())
    {
        // std::cout << "IT'S index ";
        if (tokens.size() > 1)
        {
            tokens.erase(tokens.begin());
            newServer.index = tokens;
        }
        else
            return (1);
    }
    else if(tokens.at(0).compare("error_pages")  == 0)
    {
        // std::cout << "IT'S error_pages ";
        if (tokens.size() == 3)
        {          
            int error;         
            std::map<std::string, std::string>::iterator iter;

            tokens.erase(tokens.begin());
            if (is_string_number(tokens.at(0)))  // check number
                return (1);
            if (tokens.at(0).length() != 3)     // more checks 
                return (1);
            error = atoi(tokens.at(0).c_str());

            iter = codeDescription.find(tokens.at(0));
            if (iter == codeDescription.end())
                return (1);
            // if find the key, return error else true

            if (tokens.at(1)[0] != '/')     // Check for Absolute Path   // NO Absolute Path
                return (1);
            newServer.Error_page.insert(std::pair<int, std::string>(error, tokens.at(1))); // insert element on map
        }
        else
            return (1);
    }
    else if(tokens.at(0).compare("autoindex")  == 0 && newServer.autoindex.empty())
    {
        // std::cout << "IT'S autoindex \n";
        if(tokens.size() == 2 && (!tokens.at(1).compare("on") || !tokens.at(1).compare("off")))
            newServer.autoindex = tokens.at(1);
        else
            return(1);
    }
    else if(tokens.at(0).compare("client_max_body_size")  == 0 &&  newServer.client_max_body_size == -1)
    {
        // std::cout << "IT'S client_max_body_size ";
        if(tokens.size() == 2)
        {
            if (is_string_number(tokens.at(1)))
                return (1);
            //tokens.resize(tokens.size() - 1); // atoi will take the numbe without the M;// kant mcommontya 9bal;
            //newServer.client_max_body_size =  atoi(tokens.at(1).c_str()) * 1048576;
            newServer.client_max_body_size = atoi(tokens.at(1).c_str());
        }
        else
            return(1);
    }
    else if(tokens.at(0).compare("allowed_methods")  == 0 && newServer.allowed_methods.empty())
    {
        // std::cout << "IT'S index ";
        if (tokens.size() > 1)
        {
            tokens.erase(tokens.begin());

            std::vector<std::string>::iterator it;
            for(it = tokens.begin(); it != tokens.end(); ++it)
            {
                if ((*it) != "GET" && (*it) != "POST" && (*it) != "DELETE")
                    return (1);
            }
            newServer.allowed_methods = tokens;
        }
        else
            return (1);
    }
    else
    {
        // std::cout << tokens.at(0) << ": Error Server Directive\n";
        return(1);
    }

    // std::cout << ": SUCCESS SERVER DIRECTVE\n";
    
    return(0);

}
