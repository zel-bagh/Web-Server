# ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Request.hpp"
#include "Response.hpp"


class Client {
	private:
		int sockid;
		Client(const Client& instance);
		Client& operator=(const Client& instance);

	public:
		size_t	beginning_time;
		ListenDirective	listen;
		Request request;
		Response response;

		// getters
		const int& getSockid() const;

		// Canonical form
		Client(int sockid);
		~Client();
};

#endif