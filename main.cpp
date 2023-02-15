#include "tools_cpp.hpp"

int main(int argc, char *argv[])
{
	if (argc == 2)
	{
		try {
				signal(SIGPIPE, SIG_IGN);
				store_mimeTypes();
				store_codeDescription();
				
				std::vector<ServerBlock> servers;
				if (parsingConfigFile(argv[1], servers) == 1)
					return (1);

				// Printing servers
				/*std::vector<ServerBlock>::iterator iter;
                for (iter = servers.begin(); iter != servers.end(); ++iter)
                {
                    std::cout << "---------NEW SERVER----------\n";
                        printServer(*iter);
                    std::cout << "---------END SERVER-----------\n";
                }*/

				networkProgramming(servers);
				//std::cout << "DONE" << std::endl;
		}
		catch (...) {
			std::cout << "" << std::endl;
		}
	}
	else
		std::cout << "Bad number of args" << std::endl;
	return (0);
}
