#include"./File_hpp/chunk.hpp"

chunk::chunk()
{
    line = 1;
    last_chunk = 0;
}

chunk::~chunk(){}


chunk::chunk(const chunk& obj)
{
    *this = obj;
}

chunk& chunk::operator=(const chunk& obj)
{
    line = obj.line;
    last_chunk = obj.last_chunk;
    line_content = obj.line_content;
    return *this;
}
