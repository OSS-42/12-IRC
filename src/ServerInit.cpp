#include "../inc/Server.hpp"

Server::Server(string pass) : m_on(1), m_port(6667), m_nbFds(1), m_newSD(-1), m_endServer(FALSE), m_closeConn(FALSE), m_compressArray(FALSE), m_password(pass) {
	cout << GRN "########## Launching Server ##########" NC << endl;
	gServer = this;
	m_permissionList.push_back("mbertin");
	m_permissionList.push_back("anarodri");
	m_permissionList.push_back("ewurstei");
	m_partialReq = false;
	return;
}

Server::~Server() {
	m_userDB.clear();
	m_args.clear();
	m_allChannels.clear();
	cout << RED "########## Closing Server ##########" NC << endl;
	return;
}

/*
	Les paramètres de la fonction socket() sont les suivants :
​
    domain : Spécifie le domaine d'adressage utilisé par le socket. Il peut prendre des valeurs
			telles que AF_INET pour les adresses IPv4, AF_INET6 pour les adresses IPv6,
			AF_UNIX pour les adresses de domaine UNIX, etc. Dangits le cas d'un serveur IRC
			basé sur TCP/IP, vous utiliserez généralement AF_INET pour IPv4.
​
    type : Spécifie le type de socket à créer. Les types de socket courants
			incluent SOCK_STREAM pour un socket de flux orienté connexion
			(utilisé pour TCP) et SOCK_DGRAM pour un socket de datagramme
			sans connexion (utilisé pour UDP).
​
    protocol : Spécifie le protocole à utiliser pour le socket. Si la valeur est 0,
			 le protocole approprié sera choisi automatiquement en fonction du domaine
			 et du type de socket spécifiés. Par exemple, si vous spécifiez AF_INET
			 et SOCK_STREAM, le protocole TCP sera utilisé.
*/

void	Server::initSocket() {
	if ((m_socket = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
		throw std::runtime_error("initSocket() failed miserably");
	cout << CYN "✅	initSocket" NC << endl;
	return;
}

// Parametrage du socket pour qu'il soit reutilisable et qu'il soit non bloquant
void	Server::setSocket() {
	if ((setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &m_on, sizeof(m_on)) != -1)
		&& (ioctl(m_rv, FIONBIO, &m_on)) != -1
		&& (fcntl(m_socket, F_SETFL, O_NONBLOCK)) != -1) {
		cout << CYN "✅	setSocket" NC << endl;
		return;
	}
	else {
		close(m_socket);
		throw std::runtime_error("setSocket() failed miserably");
	}
}

/*
	- in6addr_any représente une adresse IPv6 spéciale appelée "any" ou "wildcard". Cette adresse est utilisée pour lier un socket à toutes les interfaces réseau disponibles sur une machine.
​
	- bind() va nous permettre d'associer notre socket à notre adresse locale et sur laquelle le serveur sera à l'écoute des connexions entrantes.
	Lorsqu'un socket est lié à une adresse et un port spécifiques, il devient disponible pour recevoir des connexions entrantes ou envoyer des données
	vers une adresse spécifiée.
*/
void	Server::bindSocket() {
	memset(&m_addr, 0, sizeof(m_addr));
	m_addr.sin6_family = AF_INET6;
	memcpy(&m_addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	m_addr.sin6_port = htons(m_port); //Le port sera donné a la compilation

	if (bind(m_socket, (struct sockaddr*)&m_addr, sizeof(m_addr)) < 0)
	{
		close(m_socket);
		throw std::runtime_error("bindSocket() failed miserably");
	}

	char	buffer[200];
	gethostname(buffer, sizeof(buffer));
	m_hostname = static_cast<string>(buffer);

	cout << CYN "✅	bindSocket" NC << endl;
}

/*
	Passage du socket en mode listen. Le socket attend de détecté des connexions
	entrante.
*/
void	Server::listenSocket() {
	if (listen(m_socket, SOMAXCONN) < 0)
		throw std::runtime_error("listenSocket() failed miserably");
	cout << CYN "✅	listenSocket" NC << endl;
	return;
}
