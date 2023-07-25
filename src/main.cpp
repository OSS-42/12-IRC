#include "../inc/IRC.hpp"
#include "../inc/Server.hpp"

Server* gServer = nullptr;

int main(int argc, char const *argv[]) {
	if (argc != 3) {
		cerr << "Invalid number of arguments. Try again.\n";
		cerr << "Ex.:\t\"./ircserv <port> <password>\"" << endl;
		return (1);
	}
	try {
		if (strcmp(argv[1], "6667"))
			return ((cerr << "Invalid port. IRC Standart: 6667." << endl), 1);
		Server srv(argv[2]);

		signal(SIGINT, Server::signalHandler);

		srv.initSocket();
		srv.setSocket();
		srv.bindSocket();
		srv.listenSocket();
		srv.processSocket();
		return 0;
	}
	catch (std::exception& e){
		cerr << RED ">>> Unexpected error : " << e.what() << " <<<" NC << endl;
	}
	cout << GRN "Bye bye." NC << endl;
}

/*
	A VOIR EN EQUIPE :

	- Est-ce qu'on peut supprimer Client.hpp et .cpp ?

	- Il faut mettre en place l'operateur = et le constructeur par copie dans server.hpp et .cpp

	- Est-ce qu'on peut supprimer CommandControl.hpp ?
*/