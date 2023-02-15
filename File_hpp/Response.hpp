# ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "tools.hpp"

class Response {
	private:
        int sockid;

    	public:
        std::fstream stream;
        size_t stream_index; // (unsinged long long), indice in file https://www.geeksforgeeks.org/size_t-data-type-c-language/
		unsigned long long int streamLength; // of stream file

        char buf[1024];
        int buf_index;
        int bufLength;

        // if there is an error no need to check the other attributes
        int error;	// 0 no error, 1 error
		int connection;	// 1 keep alive, 0 to close connection

        //std::map<std::string, std::string> code; // map for each code and the comment with it
        //std::map<std::string, std::string> content_type; // in case no error found

	//	---------------------------------

		Response(int id);
        ~Response();
		Response(const Response& obj);
        Response& operator=(const Response& obj);
        
};

#endif
