#include "tools_cpp.hpp"
  #include <cstring>

// strlen() : <cstring>
// istream.read:    https://cplusplus.com/reference/istream/istream/read/
//  method of fstream: https://linuxhint.com/use-cpp-fstream/

int sendResponse(int sockid, Response& response)
{
    //std::cout << "response.stream_index: " << response.stream_index << " | >> " << response.streamLength << " | response.buf_index: " << response.buf_index << "\n";
    if (response.stream_index == response.streamLength && response.buf_index == -1) // stream_index == 0 && streamLength == 72
    {
       //std::cout << "ennnnnnnnnd" << std::endl;
        return (1);
    }
    else
    {
        int indice;

        indice = 0;
        if (response.buf_index == -1)
        {
            std::memset( response.buf, '\0', 1024 );
            response.buf_index = 0;
            response.stream.read(response.buf, 1024);
            response.bufLength = response.stream.gcount(); // youness
           //std::cout << "Length of buf: " << response.bufLength << std::endl;
        }

        //std::cout << "before send syscall\n";
        indice = send(sockid, response.buf + response.buf_index, response.bufLength - response.buf_index, 0);
        //std::cout << "after send syscall\n";
        if (indice <= 0)  // 0 -1
        {
            //std::cout << "send()\n";
            return (2);
        }
        response.buf_index += indice;
        if (response.buf_index == response.bufLength)
        {
            response.stream_index += response.bufLength;
            response.buf_index = -1;
            response.bufLength = 0;
            if (response.stream_index == response.streamLength && response.buf_index == -1)
                return (1);
        }
    }
    return (0);
}

