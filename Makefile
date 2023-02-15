NAME = Webserv

CC = c++

FLAGS = -Wall -Wextra -Werror 

SRCS = main.cpp \
		ServerBlock.cpp \
		parsingConfigFile.cpp \
		split_line_to_tokens.cpp \
		split_token_with_del.cpp \
		checkForAcoladeLocation.cpp \
		checkForAcoladeServ.cpp \
		find_del.cpp \
		LocationBlock.cpp\
		ListenDirective.cpp \
		fill_server.cpp\
		fill_location.cpp\
		parseServer.cpp \
		parseLocation.cpp \
		printServer.cpp \
		checkSemicolon.cpp \
		checkListenDirective.cpp \
		networkProgramming.cpp \
		createSock.cpp \
		Client.cpp \
		Response.cpp \
		Request.cpp \
		chunk.cpp \
		request_data.cpp \
		parseRequest.cpp \
		sendResponse.cpp \
		chunk_method.cpp \
		generate_response.cpp \
		storeInfo.cpp \
		store_codeDescription.cpp \
		treatRequest.cpp \
		readFromSock.cpp \
		writeToSock.cpp \
		GetListenerServers.cpp \
		is_string_number.cpp \
		find_location.cpp \
		cgi_code.cpp \
		cgi_response.cpp \
		ft_time_seconds.cpp \


all: $(NAME)

$(NAME):
	$(CC) $(FLAGS) $(SRCS) -o $(NAME)

clean:
	rm -rf $(NAME)
	
re: clean all