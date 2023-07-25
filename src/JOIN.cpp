#include "../inc/Server.hpp"

/*
	Command: JOIN
	Parameters: <channel>{,<channel>} [<key>{,<key>}]
*/

/*
	Chose à faire : Verifier si il y a
*/
void	Server::commandJOIN(vector<string> &params, int fd) {
	if (params.size() >= 2) {  // Si l'entrée à assez de parametres
		vector<Channel>::iterator channelIt = m_allChannels.begin();
		for (; channelIt != m_allChannels.end(); ++channelIt) {
			if ((*channelIt).getChanName().compare(params[1]) == 0)
				break;
		}
		joinProcess(params, fd, channelIt);
	}
	else if (params.size() < 2) {
		string buffer = ":" + m_hostname + " " + std::to_string(ERR_NEEDMOREPARAMS) + " " + m_userDB[fd].m_nickname + " JOIN :Not enough parameters\r\n";
		log(buffer);
		if (send(fd, buffer.c_str(), buffer.size(), 0) < 0)
			cerr << "send() failed" << endl;
	}
}

void	Server::joinProcess(vector<string> &params, int fd, vector<Channel>::iterator channelIt) {
	string buffer = ":" + m_userDB[fd].m_nickname + "!" + m_userDB[fd].m_username + "@" + m_hostname + " JOIN " + params[1] + "\r\n";
	Channel channel(params[1], fd);

	if (m_allChannels.empty()) //Si c'est la premier channel je l'ajoute
		firstChannel(params, fd, channel, buffer);
	else if (channelIt == m_allChannels.end()) // Si it est a la fin alors le canal n'existe pas et je l'ajoute
		newChannel(params, fd, channel, buffer);
	else if (channelIt->fdIsInChannel(fd) == false) //Le channel existe. J'ajoute l'utilisateur a la base de donnée du canal si il n'est pas dedans
		addUserToChannel(params, fd, channelIt, buffer);
}

void	Server::firstChannel(vector<string> &params, int fd, Channel channel, string buffer){
	if (params[1][0] != '#') {
		string errorMessage = ":" + m_hostname + " " + std::to_string(ERR_UNKNOWNERROR) + " " + m_userDB[fd].m_nickname + " :Missing # at the begining of channel name\r\n";
		log(errorMessage);
		if (send(fd, errorMessage.c_str(), errorMessage.size(), 0) < 0)
			cerr << "Erreur lors de l'envoi du message d'erreur ERR_UNKNOWNERROR" << endl;
	}
	else {
		log(buffer);
		if (send(fd, buffer.c_str(), buffer.size(), 0) < 0)
			cerr << "send() failed" << endl;
		else {
			if (params.size() > 2)
				channel.addKeyChannel(params);
			m_allChannels.push_back(channel);
		}
	}
}

void	Server::newChannel(vector<string> &params, int fd, Channel channel, string buffer) {
	if (params[1][0] != '#') {
		string errorMessage = ":" + m_hostname + " " + std::to_string(ERR_UNKNOWNERROR) + " " + m_userDB[fd].m_nickname + " :Missing # at the begining of channel name\r\n";
		log(errorMessage);
		if (send(fd, errorMessage.c_str(), errorMessage.size(), 0) < 0)
  			cerr << "Erreur lors de l'envoi du message d'erreur ERR_UNKNOWNERROR" << endl;
	}
	else {
		log(buffer);
		if (send(fd, buffer.c_str(), buffer.size(), 0) < 0)
			cerr << "send() failed" << endl;
		else {
			if (params.size() > 2)
				channel.addKeyChannel(params);
			m_allChannels.push_back(channel);
			m_userDB[fd].m_channels.push_back(params[1]);
		}
	}
}

void	Server::addUserToChannel(vector<string> &params, int fd, vector<Channel>::iterator channelIt, string buffer) {
	if (channelIt->getFlagInvitOnly() == TRUE) // Je verifie si le channel est en mode invitation seulement
		inviteOnlyChan(params, fd, channelIt);
	else if (channelIt->getFlagChanPass() == TRUE && channelIt->checkKeyChannel(params) == false)// Je verifie si il y a un mot de passe
		badChannelKey(params, fd);
	else if (channelIt->isChannelFull() == true) // Je verifie si le channel a une limite et si il est plein
		channelIsFull(params, fd, channelIt);
	else {
		log(buffer);
		if (send(fd, buffer.c_str(), buffer.size(), 0) < 0)
			cerr << "send() failed" << endl;
		else {
			(*channelIt).changeNbrMember("+");
			(*channelIt).addFd(fd);
			m_userDB[fd].m_channels.push_back(params[1]);
		}
	}
}

void	Server::printAllChannel() {
		for (vector<Channel>::iterator it = m_allChannels.begin(); it != m_allChannels.end(); it++) {
				cout << YEL "Channel name: " << (*it).getChanName() << endl;
				cout << "Topic mode : " << it->getFlagOperOnlyTopic() << endl;
				cout << "Channel topic: " << (*it).getTopic() << endl;
				cout << "Channel password mode: " << (*it).getFlagChanPass() << endl;
				cout << "Channel password : " << (*it).getChanPass() << endl;
				cout << "Channel invit only mode: " << (*it).getFlagInvitOnly() << endl;
				cout << "Number of members: " << (*it).getNbrMember() << endl;
				cout << "Fd members: " << endl;
				(*it).displayFdList();
				cout << YEL "Fd operator members OF THE CHANNEL: " << endl;
				(*it).displayOperFdList();
				// Print other channel information as needed
				// Il faudrait voir pour afficher les operateur global
		}
}
// :nickname!username@hostname JOIN #channel\r\n
// :mbertin!morgan@c2r5p2.42quebec.com JOIN #general
// :dan-!d@localhost JOIN #test
// :server_name 475 <nickname> <channel> :Bad Channel Key
