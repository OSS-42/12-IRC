#include "../inc/Server.hpp"

/*
	Lorsque POLLIN est défini pour un descripteur de fichier donné dans la structure pollfd, la fonction poll() vérifie si ce descripteur de fichier est prêt à être lu. Si c'est le cas, le champ revents de la structure pollfd correspondante sera mis à POLLIN, indiquant que des données peuvent être lues sans être bloqué.
*/
void	Server::processSocket() {
	memset(m_fds, 0, sizeof(m_fds));
	m_fds[0].fd = m_socket;
	m_fds[0].events = POLLIN; // POLLIN, indique que nous souhaitons surveiller si des données sont prêtes à être lue.
	cout << CYN "\n>>> Server up and ready <<<\n" NC << endl;
	cout << CYN "Hostname : " << m_hostname << "\n" << endl;
	do
	{
		m_rv = poll(m_fds, m_nbFds, TIMEOUT); //TIMEOUT definition: IRC.hpp
		if (m_rv < 0)
		{
			perror("poll() failed");
			break;
		}
		if (m_rv == 0)
		{
			cout << RED "poll() timed out.  End program." NC << endl;
			break;
		}
		acceptReceive();
		if (m_compressArray)
			compressArray();
	} while (m_endServer == FALSE);
	closeServer();
}

void	Server::acceptReceive() {
	int	current_size = m_nbFds;
	for (int i = 0; i < current_size; ++i) {

		if(m_fds[i].revents == 0)
			continue;

		if(m_fds[i].revents != POLLIN && m_fds[i].revents != 17)
		{
			cerr << "Error. revents = " << m_fds[i].revents << endl;
			m_endServer = TRUE;
			break;
		}

		//Si revents != 0 et != POLLIN alors ca veut dire qu'on a recu une requete
		if (m_fds[i].fd == m_socket) { //Le serveur accepte les requetes.
			acceptSocket();
		}
		else { //Reception de la requete accepté
			receiveSocket(i);
			if (m_closeConn) // on a recu la fermeture du client qu'on a recv()
				closeConnection(i);
		} //Reception de la requete accepté

		if (m_userDB[m_fds[i].fd].m_flagWelcome == FALSE && !m_userDB[m_fds[i].fd].m_nickname.empty() && !m_userDB[m_fds[i].fd].m_username.empty()) {
			cout << MAG "checks done" NC << endl;
			welcomeMessage(m_fds[i].fd);
			m_userDB[m_fds[i].fd].m_flagWelcome = TRUE;
		}
	}
}

void	Server::acceptSocket() {
	do
	{
		m_newSD = accept(m_socket, NULL, NULL);
		if (m_newSD < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				perror("accept() failed");
				m_endServer = TRUE;
			}
			break;
		}
		cout << MAG "New request accepted\n" NC << endl;
		m_fds[m_nbFds].fd = m_newSD;
		m_fds[m_nbFds].events = POLLIN;
		m_nbFds++;
	} while (m_newSD != -1);
}

void	Server::receiveSocket(int &i) {
	do
	{
		m_rv = recv(m_fds[i].fd, m_buffer, sizeof(m_buffer), 0);
		if (m_rv < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				perror("recv() failed");
				m_closeConn = TRUE;
			}
			break;
		}
		if (m_rv == 0)
		{
			cout << "User, descriptor #" << m_fds[i].fd << ", wants to close connexion with the server" << endl;
			m_closeConn = TRUE;
			break;
		}

 		if (m_rv > 512) {
			m_buffer[511] = '\0';
        }
		else {
          m_buffer[m_rv] = '\0';
        }
		
		if (m_buffer[0]) {
			cerr << GRN "\t[" << getTimestamp() << "] Server Received: " NC << m_buffer << endl;
			splitBuffer(m_buffer);
			if (m_partialReq == false) {
				if (m_userDB[m_fds[i].fd].m_flagWelcome == FALSE && m_userDB[m_fds[i].fd].m_flagPass == FALSE) {
					checkPASS(m_args, m_fds[i].fd);
				}
				else if (m_userDB[m_fds[i].fd].m_flagWelcome == FALSE && m_userDB[m_fds[i].fd].m_flagPass == TRUE) {
					storeBufferValue(m_args, m_fds[i].fd);
				} else if (m_userDB[m_fds[i].fd].m_flagWelcome == TRUE) {
					execCommand(m_args, m_fds[i].fd);
				}
			}
		}
		memset(&m_buffer, 0, sizeof(m_buffer));
	} while (TRUE);
}

void	Server::compressArray(void) {
	m_compressArray = FALSE;
	cout << YEL "Compressing the array of fd ..." << endl;
	for (int i = 0; i < m_nbFds; i++)
	{
		if (m_fds[i].fd == -1) {
			for(int j = i; j < m_nbFds; j++) {
				m_fds[j].fd = m_fds[j + 1].fd;
			}
			i--;
			m_nbFds--;
		}
	}
	cout << GRN "Array of fd compressed" NC << endl;
}


void	Server::closeConnection(int &i) {
	cout << YEL "Closing connexion ..." << endl;
	m_compressArray = TRUE;
	m_userDB[m_fds[i].fd].m_flagPass = 0;
	m_userDB[m_fds[i].fd].m_flagWelcome = 0;
	m_userDB[m_fds[i].fd].m_username = "\0";
	m_userDB[m_fds[i].fd].m_nickname = "\0";
	m_userDB[m_fds[i].fd].m_channels.clear();
	close(m_fds[i].fd);
	m_closeConn = FALSE;
	for (vector<Channel>::iterator channelIt = m_allChannels.begin(); channelIt != m_allChannels.end(); channelIt++)
			channelIt->eraseFdFromList(m_fds[i].fd);
	m_fds[i].fd = -1;
	cout << GRN "Connexion closed" NC << endl;
}

void	Server::closeServer(void) {
	cout << YEL "Cleaning the server ..." NC << endl;
	for (int i = 1; i < m_nbFds; i++) {
		if(m_fds[i].fd >= 0)
			close(m_fds[i].fd);
	}
	close(m_fds[0].fd);
	cout << GRN "The server is now close" NC << endl;
}