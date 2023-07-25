#include "../inc/Server.hpp"

/*
Command: MODE <channel> ( "-" / "+" ) <mode> <modeparams>

		i - set/remove invite-only channel;
		t - set/remove restrictions of TOPIC command to chanOPs only
		k - set/remove channel key
		o - give/take channel operator privilege
		l - set/remove user limit to channel
*/

void	Server::commandMODE(vector<string> &params, int fd) {
	if (params.size() < 3)
		notEnoughtParams(params, fd);
	else
		modeProcess(params, fd);
}

void	Server::modeProcess(vector<string> &params, int fd){
	if (params.at(2) == "+l" || params.at(2) == "-l") {
		modeL(params, fd);
	}
	else if (params.at(2) == "+i" || params.at(2) == "-i") {
		modeI(params, fd);
	}
	else if (params.at(2) == "+t" || params.at(2) == "-t") {
		modeT(params, fd);
	}
	else if (params.at(2) == "+k" || params.at(2) == "-k") {
		modeK(params, fd);
	}
	else if (params.at(2) == "+o" || params.at(2) == "-o") {
		modeO(params, fd);
	}
	else {
		string errorMessage = ":" + m_hostname + " " + std::to_string(ERR_UNKNOWNMODE) + " " + m_userDB[fd].m_nickname + " " + params[2] + " :Unknown mode\r\n";
		log(errorMessage);
		if (send(fd, errorMessage.c_str(), errorMessage.size(), 0) < 0)
  			cerr << "send() failed" << endl;
	}
}

//	---------------------------------- MODE L ---------------------------------

// MODE #channel +l <limits> : Ajoute une limite d'utilisateur au channel
// MODE #channel -l : Retire la limite d'utilisateur du channel
void		Server::modeL(vector<string> &params, int fd) {
	vector<Channel>::iterator channel = channelIsInServer(params[1]);
	if (channel == m_allChannels.end())
		noSuchChannel(params, fd, channel);
	else if (channel->fdIsInChannel(fd) == false)
		notInChannel(params, fd);
	else if (m_userDB[fd].m_isOP == false && channel->operatorIsInChannel(fd) == false)
		needOperPriv(params, fd);
	else {
		if (params.at(2) == "+l") {
			if (params.size() < 4)
				notEnoughtParams(params, fd);
			else if (std::stoi(params[3]) <= 0)
				badNumericParam(params, fd);
			else
				positiveL(params, fd, channel);
		}
		else if(params.at(2) == "-l")
				negativeL(params, fd, channel);
	}
}

void		Server::positiveL(vector<string> &params, int fd, vector<Channel>::iterator channel) {
	string successMessage = ":" + m_hostname + " " + std::to_string(RPL_CHANNELMODEIS) + " " + channel->getChanName() + " +l :User limit is now set to " + params[3] + " in channel " + channel->getChanName() + "\r\n";
	log(successMessage);
	if (send(fd, successMessage.c_str(), successMessage.size(), 0) < 0)
		cerr << "send() failed" << endl;
	else
		channel->setMemberLimit(std::stoi(params[3]));
}


void		Server::negativeL(vector<string> &params, int fd, vector<Channel>::iterator channel) {
	string successMessage = ":" + m_hostname + " " + std::to_string(RPL_CHANNELMODEIS) + " " + channel->getChanName() + " -l :Channel " + params[1] + " have no limit of user\r\n";
	log(successMessage);
	if (send(fd, successMessage.c_str(), successMessage.size(), 0) < 0)
		cerr << "send() failed" << endl;
	else
		channel->setMemberLimit(0);
}


//	---------------------------------- MODE I ---------------------------------

// MODE #channel +i : Active le mode invitation seulement pour rentrer dans le channel
// MODE #channel -i : Desactive le mode invitation seulement pour rentrer dans le channel
void		Server::modeI(vector<string> &params, int fd) {
	vector<Channel>::iterator channel = channelIsInServer(params[1]);
	if (channel == m_allChannels.end())
		noSuchChannel(params, fd, channel);
	else if (channel->fdIsInChannel(fd) == false)
		notInChannel(params, fd);
	else if (m_userDB[fd].m_isOP == false && channel->operatorIsInChannel(fd) == false)
		needOperPriv(params, fd);
	else if (params[2] == "+i")
		positiveI(params, fd, channel);
	else if (params[2] == "-i")
		negativeI(params, fd, channel);
}

void	Server::positiveI(vector<string> &params, int fd, vector<Channel>::iterator channel) {
	string successMessage = ":" + m_hostname + " " + std::to_string(RPL_CHANNELMODEIS) + " " + params[1] + " +i :Invite-only mode activate for the channel " + params[1] + "\r\n";;
	log(successMessage);
	if (send(fd, successMessage.c_str(), successMessage.size(), 0) < 0)
		cerr << "send() failed" << endl;
	else {
		channel->setFlagInvitOnly(TRUE);
	}
}

void	Server::negativeI(vector<string> &params, int fd, vector<Channel>::iterator channel) {
	string successMessage = ":" + m_hostname + " " + std::to_string(RPL_CHANNELMODEIS) + " " + params[1] + " -i :Invite-only mode deactivate for the channel " + params[1] + "\r\n";
	log(successMessage);
	if (send(fd, successMessage.c_str(), successMessage.size(), 0) < 0)
		cerr << "send() failed" << endl;
	else {
		channel->setFlagInvitOnly(FALSE);
	}
}

//	---------------------------------- MODE T ---------------------------------

//MODE #channel +t  : le topic peut être modifié par n'importe quel utilisateur du channel
//MODE #channel -t : le topic peut être modifié seulement par un operateur
void		Server::modeT(vector<string> &params, int fd) {
	vector<Channel>::iterator channel = channelIsInServer(params[1]);
	if (channel == m_allChannels.end())
		noSuchChannel(params, fd, channel);
	else if (channel->fdIsInChannel(fd) == false)
		notInChannel(params, fd);
	else if (m_userDB[fd].m_isOP == false && channel->operatorIsInChannel(fd) == false)
		needOperPriv(params, fd);
	else {
		if (params.at(2) == "+t")
			positiveT(params, fd, channel);
		else if (params.at(2) == "-t")
			negativeT(params, fd, channel);
	}
}

void		Server::positiveT(vector<string> &params, int fd, vector<Channel>::iterator channel) {
	string successMessage = ":" + m_hostname + " " + std::to_string(RPL_CHANNELMODEIS) + " " + m_userDB[fd].m_nickname + " " + params[1] + " +t. The topic can be change by any user.\r\n";
	log(successMessage);
	if (send(fd, successMessage.c_str(), successMessage.size(), 0) < 0)
		cerr << "send() failed" << endl;
	else
		channel->setFlagOperOnlyTopic(FALSE);
}

void		Server::negativeT(vector<string> &params, int fd, vector<Channel>::iterator channel) {
	string successMessage = ":" + m_hostname + " " + std::to_string(RPL_CHANNELMODEIS) + " " + m_userDB[fd].m_nickname + " " + params[1] + " -t. The topic can be change only by a operator.\r\n";
	log(successMessage);
	if (send(fd, successMessage.c_str(), successMessage.size(), 0) < 0)
		cerr << "send() failed" << endl;
	else
		channel->setFlagOperOnlyTopic(TRUE);
}

//	---------------------------------- MODE K ---------------------------------

// MODE #channel +k mot de passe : Donne un mot de passe au channel
// MODE #channel -k : Retire le mot de passe du channel
void		Server::modeK(vector<string> &params, int fd) {
	vector<Channel>::iterator channel = channelIsInServer(params[1]);
	if (channel == m_allChannels.end())
		noSuchChannel(params, fd, channel);
	else if (channel->fdIsInChannel(fd) == false)
		notInChannel(params, fd);
	else if (m_userDB[fd].m_isOP == false && channel->operatorIsInChannel(fd) == false)
		needOperPriv(params, fd);
	else {
		if (params.at(2) == "+k") {
			if (params.size() < 4)
				notEnoughtParams(params, fd);
			else
				positiveK(params, fd, channel);
		}
		else if (params.at(2) == "-k")
			negativeK(params, fd, channel);
	}
}

void		Server::positiveK(vector<string> &params, int fd, vector<Channel>::iterator channel) {
	string key = params[3];
	for (vector<string>::iterator keyIt = params.begin() + 4; keyIt != params.end(); ++keyIt)
		key += " " + (*keyIt);
	string successMessage = ":" + m_hostname + " " + std::to_string(RPL_CHANNELMODEIS) + " " + m_userDB[fd].m_nickname + " " + params[1] + " +k " + key + "\r\n";
	log(successMessage);
	if (send(fd, successMessage.c_str(), successMessage.size(), 0) < 0)
		cerr << "send() failed" << endl;
	else {
		channel->setChanPass(key);
		channel->setFlagChanPass(TRUE);
	}
}

void		Server::negativeK(vector<string> &params, int fd, vector<Channel>::iterator channel) {
	string successMessage = ":" + m_hostname + " " + std::to_string(RPL_CHANNELMODEIS) + " " + m_userDB[fd].m_nickname + " " + params[1] + " -k\r\n";
	log(successMessage);
	if (send(fd, successMessage.c_str(), successMessage.size(), 0) < 0)
		cerr << "send() failed" << endl;
	else {
			channel->setFlagChanPass(FALSE);
			channel->setChanPass("\0");
	}
}

//	---------------------------------- MODE O ---------------------------------

// "MODE #channel +o nickname" Donne le statut operateur a nickname
// "MODE #channel -o nickname" Enleve le statut operateur a nickname
void		Server::modeO(vector<string> &params, int fd) {
	vector<Channel>::iterator channel = channelIsInServer(params[1]);
	if (channel == m_allChannels.end())
		noSuchChannel(params, fd, channel);
	else if (channel->fdIsInChannel(fd) == false)
		notInChannel(params, fd);
	else if (m_userDB[fd].m_isOP == false && channel->operatorIsInChannel(fd) == false)
		needOperPriv(params, fd);
	else if (params.size() < 4)
		notEnoughtParams(params, fd);
	else {
		if (channel->fdIsInChannel(getFdFromNickname(params[3])) == false)
			targetNotInChannel(params, fd, channel);
		else if (params.at(2) == "+o")
			positiveO(params, fd, channel);
		else if (params.at(2) == "-o")
			negativeO(params, fd, channel);
	}
}

void	Server::positiveO(vector<string> &params, int fd, vector<Channel>::iterator channel) {
	string successMessage = ":" + m_hostname + " " + std::to_string(RPL_YOUREOPER) + " " + m_userDB[fd].m_nickname + " You are now an IRC operator of the channel " + params[1] + "\r\n";
	log(successMessage);
	if (send(getFdFromNickname(params[3]), successMessage.c_str(), successMessage.size(), 0) < 0)
		cerr << "send() failed" << endl;
	else
		channel->addOperToChannel(getFdFromNickname(params[3]));
}

void	Server::negativeO(vector<string> &params, int fd, vector<Channel>::iterator channel) {
	string successMessage = ":" + m_hostname + " " + std::to_string(RPL_YOUREOPER) + " " + m_userDB[fd].m_nickname + " You are no longer an IRC operator of the channel " + params[1] + "\r\n";
	log(successMessage);
	if (send(getFdFromNickname(params[3]), successMessage.c_str(), successMessage.size(), 0) < 0)
		cerr << "send() failed" << endl;
	else
		channel->eraseOperFromChannel(getFdFromNickname(params[3]));
}