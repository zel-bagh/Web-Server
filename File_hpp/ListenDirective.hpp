# ifndef LISTENDIRECTIVE_HPP
#define LISTENDIRECTIVE_HPP

#include "tools.hpp"

class ListenDirective {
	public:
		bool it_exist;	// True or False
		std::string host; // string
		int	port;	  // int

		// Canonical form
		ListenDirective();
		~ListenDirective();
		ListenDirective(const ListenDirective& instance);
		ListenDirective& operator=(const ListenDirective& instance);
		bool operator<(const ListenDirective& instance) const;	
};

#endif