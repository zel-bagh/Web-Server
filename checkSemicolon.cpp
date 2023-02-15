#include "tools_cpp.hpp"

int checkSemicolon(std::vector<std::string>& tokens)
{
    std::string str;
    std::vector<std::string>::iterator iter;

    str = tokens.at(tokens.size() - 1);
    if (find_del(str, ';') != 1)
        return (1);
    if (str[str.length() - 1] != ';')
        return (1);
    tokens.at(tokens.size() - 1) = str.substr(0, str.length() - 1);
    for(iter = tokens.begin(); iter != tokens.end(); ++iter)
    {
        if(find_del(*iter, ';'))
            return(1);
    }
    return (0);
}
