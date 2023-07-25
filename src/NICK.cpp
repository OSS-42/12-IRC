#include "../inc/Server.hpp"

void	Server::commandNICK(vector<string> &params, int fd) {

	std::ostringstream oss;
	string message;

	oss.clear();
	oss.str("");
	if (params.at(1).empty() == TRUE) {
		oss << ":" << m_hostname << " 431 nonick :No nickname given\r\n";
		message = oss.str();
	}

	else if (isNickOK(params.at(1)) == FALSE ) {
		oss << ":"  << m_hostname << " 432 * :Erroneous nickname\r\n";
		message = oss.str();
	}

	else if (isNickTaken(params.at(1)) == TRUE) {
		oss << ":" << m_hostname << " 433 * :Nickname is already in use\r\n";
		message = oss.str();
	}

	else {
		oss << ":" << m_userDB[fd].m_nickname << "!" << m_userDB[fd].m_username << "@" << m_hostname  << " NICK " << params.at(1) << "\r\n";
		message = oss.str();
		m_userDB[fd].m_nickname = params.at(1);
	}

	log(message);
	if (send(fd, message.c_str(), message.size(), 0) < 0)
			perror("CMD NICK : message sending failed");
}

bool	Server::isNickTaken(string nickname) {
	for (map<int, clientInfo>::iterator it = m_userDB.begin(); it != m_userDB.end(); ++it) {
		if (it->second.m_nickname == nickname)
			return TRUE;
	}
	return FALSE;
}

bool	Server::isNickOK(string nickname) {

	const string characters = "abcdefghijklmonopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_[]\\`^{}";

	if (nickname.size() > 9)
		return FALSE;
	else if (nickname.find_first_not_of(characters) != string::npos)
		return FALSE;
	else
		return TRUE;
}