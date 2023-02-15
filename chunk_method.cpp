#include"tools_cpp.hpp"

int hexstr_toint(std::string str)
{
    int ret = 0;
    size_t i = 0;

    while(i != str.size())
    {
        if (str[i] >= '0' && str[i] <= '9')
            ret = (ret*16) + (str[i] - 48);
        else if (str[i] >= 'A' && str[i] <= 'F')
            ret = (ret*16) + (str[i] - 55);
        else if (str[i] >= 'a' && str[i] <= 'f')
            ret = (ret*16) + (str[i] - 87);
        i++;
    }
    return ret;
}

void chunk_body(Client &packet, std::fstream& fs, int store)
{
    size_t i;
    int j;

    while(1) 
    {
        if (packet.request.chk.line == 1)
        {
            i = 0;
            while(i != packet.request.content.size())
            {
                if (packet.request.chk.line_content.size() != 0 &&
                    packet.request.chk.line_content[packet.request.chk.line_content.size() - 1] == '\r' && packet.request.content[i] != '\n')
                {
                    if(store)
                        fs.close();
                    packet.response.error = 400;
                    throw 1;
                }
                else if (packet.request.chk.line_content.size() != 0 &&
                    packet.request.chk.line_content[packet.request.chk.line_content.size() - 1] == '\r' && packet.request.content[i] == '\n')
                {
                    packet.request.chk.line_content.erase(packet.request.chk.line_content.size() - 1);
                    break ;
                }
                if (packet.request.content[i] == '\r' && (i + 1 != packet.request.content.size()) && packet.request.content[i + 1] == '\n' && ++i)
                    break;
                else if (packet.request.content[i] == '\r' && (i + 1 != packet.request.content.size()) && packet.request.content[i + 1] != '\n')
                {
                    if(store)
                        fs.close();
                    packet.response.error = 400;
                    throw 1;
                }
                if ((packet.request.content[i] < '0' || packet.request.content[i] > '9')
                    && (packet.request.content[i] < 'A' || packet.request.content[i] > 'F') 
                    && (packet.request.content[i] < 'a' || packet.request.content[i] > 'f') && packet.request.content[i] != '\r')
                {
                    if(store)
                        fs.close();
                    packet.response.error = 400;
                    throw 1;
                }
                i++;
            }
            if (i == packet.request.content.size())
            {
                packet.request.chk.line_content.append(packet.request.content);//packet.request.content.substr(0, i));
                packet.request.content.clear();
                throw 0;
            }
            else if (packet.request.content[i] == '\n')
            {
                j = (i != 0) ? i : 1; 
                packet.request.chk.line_content.append(packet.request.content.substr(0, j - 1));
                packet.request.chk.line_length = hexstr_toint(packet.request.chk.line_content);
                packet.request.chk.line = 2;
                packet.request.chk.line_content.clear();
                packet.request.content.erase(0, i + 1);
                if (packet.request.chk.line_length == 0)
                    packet.request.chk.last_chunk = 1;
            }
        }
        if (packet.request.chk.line == 2 || packet.request.chk.line == 3)
        {
            i = 0;
            while (i != packet.request.content.size())
            {
                if ((packet.request.chk.line_length == 0 && packet.request.chk.line == 3 && packet.request.content[i] != '\n') ||
                    (packet.request.chk.line_length == 0 && packet.request.chk.line == 2 && packet.request.content[i] != '\r'))
                {
                    if(store)
                        fs.close();
                    packet.response.error = 400;
                    throw 1;
                }
                if (packet.request.chk.line_length == 0 && packet.request.chk.line == 3 && packet.request.content[i] == '\n')
                    break ;
                if (packet.request.chk.line_length == 0 && packet.request.content[i] == '\r')
                       packet.request.chk.line = 3;
                if(packet.request.chk.line_length != 0)
                    fs << packet.request.content[i];
                if (packet.request.chk.line_length != 0)
                    packet.request.chk.line_length--;
                i++;
            }
            if (i == packet.request.content.size())
            {
                packet.request.content.clear();
                throw 0;
            }
            else if (packet.request.content[i] == '\n')
            {
                packet.request.chk.line = 1;
                packet.request.content.erase(0, i + 1);
            }
            if (packet.request.chk.last_chunk)
                break;
        }
    }
    if (store)
        fs.close();
    throw 1;
}