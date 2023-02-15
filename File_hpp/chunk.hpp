#ifndef CHUNK_hpp
#define CHUNK_hpp

#include<string>

class chunk
{
    public:
        int line;
        std::string line_content;
        int line_length;
        int last_chunk;

        // canonical
        chunk();
        chunk(const chunk& obj);
        chunk& operator=(const chunk& obj);
        ~chunk();
};



#endif

//2\r\nab\r\n0\r\n\r\n