#include "tools_cpp.hpp"

int is_string_number(std::string& str)  //probably a refrence
{
    std::string::iterator str_it;
	int i;

    for(str_it = str.begin(); str_it != str.end(); ++str_it)
    {
        if(!isdigit(*str_it))
            return(1);
    }
	i = 0;
	for(str_it = str.begin(); str_it != str.end(); ++str_it)
    {
		if ((*str_it) == '0')
			i++;
		else
			break ;
    }
	str = str.substr(i);
    return(0);
}
