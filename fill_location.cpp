#include "tools_cpp.hpp"

int fill_location(std::vector<std::string> tokens, LocationBlock &newLocation)
{
    if (checkSemicolon(tokens))
        return (1);

    // std::cout << tokens.at(0) << std::endl;

    if (tokens.at(0).compare("root") == 0 && newLocation.root.empty())
    {
        // std::cout << "IT'S root ";
        if (tokens.size() == 2 && tokens.at(1)[0] == '/') // Check for Absolute Path
            newLocation.root = tokens.at(1);
        else
            return (1);
    }
    else if (tokens.at(0).compare("index") == 0 && newLocation.index.empty())
    {
        // std::cout << "IT'S index ";
        if (tokens.size() > 1)
        {
            tokens.erase(tokens.begin());
            newLocation.index = tokens;
        }
        else
            return (1);
    }
    else if (tokens.at(0).compare("error_pages") == 0)
    {
        // std::cout << "IT'S error_pages ";
        if (tokens.size() == 3)
        {
            int error;
            std::map<std::string, std::string>::iterator iter;

            tokens.erase(tokens.begin());
            if (is_string_number(tokens.at(0))) // check number
                return (1);
            if (tokens.at(0).length() != 3) // more checks
                return (1);
            error = atoi(tokens.at(0).c_str());

            iter = codeDescription.find(tokens.at(0));
            if (iter == codeDescription.end())
                return (1);

            // if find the key, return error else true

            if (tokens.at(1)[0] != '/') // Check for Absolute Path // NO Absolute Path
                return (1);
            newLocation.Error_page.insert(std::pair<int, std::string>(error, tokens.at(1))); // insert element on map
        }
        else
            return (1);
    }
    else if (tokens.at(0).compare("allowed_methods") == 0 && newLocation.allowed_methods.empty())
    {
        // std::cout << "IT'S index ";
        if (tokens.size() > 1)
        {
            tokens.erase(tokens.begin());

            std::vector<std::string>::iterator it;
            for (it = tokens.begin(); it != tokens.end(); ++it)
            {
                if ((*it) != "GET" && (*it) != "POST" && (*it) != "DELETE")
                    return (1);
            }
            newLocation.allowed_methods = tokens;
        }
        else
            return (1);
    }
    else if (tokens.at(0).compare("autoindex") == 0 && newLocation.autoindex.empty())
    {
        if (tokens.size() == 2 && (tokens.at(1) == "on" || !tokens.at(1).compare("off")))
            newLocation.autoindex = tokens.at(1);
        else
            return (1);
    }
    else if (tokens.at(0).compare("client_max_body_size") == 0 && newLocation.client_max_body_size == -1) //
    {
        // std::cout << "IT'S max_body_size ";
        if (tokens.size() == 2)
        {
            if (is_string_number(tokens.at(1)))
                return (1);
            // tokens.resize(tokens.size() - 1); //atoi will take the numbe without the M;
            // newLocation.client_max_body_size =  atoi(tokens.at(1).c_str()) * 1048576;
            newLocation.client_max_body_size = atoi(tokens.at(1).c_str());
        }
        else
            return (1);
    }
    else if (tokens.at(0).compare("upload") == 0 && newLocation.upload.empty())
    {
        // std::cout << "IT'S upload ";
        if (tokens.size() == 2 && tokens.at(1)[0] == '/') // Check for Absolute Path
            newLocation.upload = tokens.at(1);
        else
            return (1);
    }
    else if (tokens.at(0).compare("redirection") == 0 && newLocation.redirection.empty())
    {
        // std::cout << "IT'S redirection ";
        if (tokens.size() == 3 ) // Check for Absolute Path
        {
            tokens.erase(tokens.begin());
            newLocation.redirection = tokens;
        }
        else
            return (1);
    }
    else if (tokens.at(0)[0] == 'c' && tokens.at(0)[1] == 'g' && tokens.at(0)[2] == 'i' && tokens.at(0)[3] == '_')
    {
        // std::cout << "IT'S CGI path ";
        if (tokens.at(0).length() == 4)
            return (1);
        if (tokens.size() == 2 && tokens.at(1)[0] == '/')                                                 // Check for Absolute Path
            newLocation.cgi_path.insert(std::pair<std::string, std::string>(tokens.at(0), tokens.at(1))); // insert element on map
        else
            return (1);
    }
    else
    {
        // std::cout << tokens.at(0) << ": Error Location Directive.\n";
        return (1);
    }

    // std::cout << ": SUCCESS LOCATION DIRECTVE\n";

    return (0);
}