#include "../inc/Server.hpp"

// INVITE <nickname> <channel>
void	Server::commandINVITE(vector<string> &params, int fd) {
	if (params.size() < 3) {
		string buffer = ":" + m_hostname + " " + std::to_string(ERR_NEEDMOREPARAMS) + " " + m_userDB[fd].m_nickname + " INVITE :Not enough parameters\r\n";
		log(buffer);
		if (send(fd, buffer.c_str(), buffer.size(), 0) < 0)
			cerr << "send() failed" << endl;
	}
	else {
		vector<Channel>::iterator chanIt = channelIsInServer(params.at(2));
		if (chanIt == m_allChannels.end())
			noSuchChannel(params, fd, chanIt);
		else if (chanIt->fdIsInChannel(fd) == false)
			notInChannel(params, fd);
		else if (fdIsInServer(getFdFromNickname(params[1])) == false)
			noSuchNick(params, fd);
		else if (chanIt->fdIsInChannel(getFdFromNickname(params[1])) == true)
			userOnChannel(params, fd);
		else
			inviteProcess(params, fd, chanIt);
	}
}

void	Server::inviteProcess(vector<string> &params, int fd, vector<Channel>::iterator channel) {
	string inviteBuffer = ":" + m_hostname + " " + std::to_string(RPL_INVITING) + " " + m_userDB[fd].m_nickname + " " + params[1] + " " + params[2] + "\r\n";
	log(inviteBuffer);
	if (send(fd, inviteBuffer.c_str(), inviteBuffer.size(), 0) < 0) // je send a l'utilisateur de INVITE
		cerr << "send() failed" << endl;
	else {
		int targetFd = getFdFromNickname(params[1]);
		string inviteNotifBuffer = ":" + m_userDB[fd].m_nickname + " INVITE " + params[1] + " " + params[2] + "\r\n";
		log(inviteNotifBuffer);
		if (send(targetFd, inviteNotifBuffer.c_str(), inviteNotifBuffer.size(), 0) < 0) // je send a l'utilisateur invité qu'il est invité
			cerr << "send() failed" << endl;
		else {
			string joinBuffer = ":" + m_userDB[targetFd].m_nickname + "!" + m_userDB[targetFd].m_username + "@" + m_hostname + " JOIN " + params[2] + "\r\n";
			log(joinBuffer);
			if (send(targetFd, joinBuffer.c_str(), joinBuffer.size(), 0) < 0)// J'ajoute l'invité dans le channel
				cerr << "send() failed" << endl;
			else {
				channel->changeNbrMember("+");
				channel->addFd(targetFd);
				m_userDB[targetFd].m_channels.push_back(params[1]);
			}
		}
	}
}
