#include "../inc/Server.hpp"

void	Server::storeBufferValue(vector<string> &params, int fd) {
	string cppBuffer = m_buffer;
	if (params.size() > 1) {
		if (params.at(0) == "NICK")
			commandNICK(params, fd);

		else if (params.at(0) == "USER") {
			clientDetection(fd, cppBuffer);
			m_userDB[fd].m_username = params.at(1);
		}

		if (!m_userDB[fd].m_nickname.empty() && !m_userDB[fd].m_username.empty()) {
			m_userDB[fd].m_flagWelcome = TRUE;
			cout << MAG "New Client connection checks done" NC << endl;
			welcomeMessage(fd);
		}
		return ;
	}
}

void	Server::welcomeMessage(int fd) {
	std::ostringstream	oss;
	oss << ":" << m_hostname << " 001 " << m_userDB[fd].m_nickname << " :Welcome to the IRCServ, " << m_userDB[fd].m_nickname << "!" << m_userDB[fd].m_username << "@" << m_hostname << "\r\n";
	string message = oss.str();
	log(message);
	if (send(fd, message.c_str(), message.size(), 0) < 0)
		perror("message sending failed");
	else
		cout << CYN ">>> Welcome message sent <<<\n" NC << endl;
}

void	Server::splitBuffer(char* buffer) {
	m_args.clear();
	if (buffer == nullptr || buffer[0] == '\0') {
        m_args.push_back("\r\n");
        return ;
    }
    else {
        string buff = buffer;
		if (buff.back() != '\n' || m_partialReq == true) 
		{
			m_tmpBuff += buff;
			m_partialReq = true;
			if (buff.back() == '\n') {
				m_partialReq = false;
				buff = m_tmpBuff;
				m_tmpBuff.clear();
				m_tmpBuff = "";
			}
		}
		if (m_partialReq == false) {
			while (!buff.empty() && (buff.back() == '\n' || buff.back() == '\r'))
				buff.pop_back(); // erases trailing \r\n from input

			std::stringstream ss (buff);
			string token;
			while (getline(ss, token, ' ')) { // separates input into token using whitespace as a delimiter
				if (token.front() != ':')
					m_args.push_back(token);
				if (!token.empty() && token.front() == ':') { // when ":" is found, everything else is parsed as a string literal
					m_args.push_back(buff.substr(buff.find(':')));
				break;            }
			}
		}
	}
	return;
}

void	Server::execCommand(vector<string> &params, int fd) {
	if (params.at(0) == "NICK")
		commandNICK(m_args, fd);
	else if (params.at(0) == "PRIVMSG")
		commandPRIVMSG(m_args, fd);
	else if (params.at(0) == "NOTICE")
		commandNOTICE(m_args, fd);
	else if (params.at(0) == "JOIN")
		commandJOIN(m_args, fd);
	else if (params.at(0) == "OPER")
		commandOPER(m_args, fd);
	else if (params.at(0) == "MODE")
		commandMODE(m_args, fd);
	else if (params.at(0) == "KICK")
		commandKICK(m_args, fd);
	else if (params.at(0) == "PART")
		commandPART(m_args, fd);
	else if (params.at(0) == "INVITE")
		commandINVITE(m_args, fd);
	else if (params.at(0) == "TIME")
		commandTIME(m_args, fd);
	else if (params.at(0) == "DIE")
		commandDIE(m_args, fd);
	else if (params.at(0) == "TOPIC")
		commandTOPIC(m_args, fd);
}

void	Server::clientDetection(int fd, string registration) {
	if (registration.find("*") != string::npos && registration.find(m_userDB[fd].m_nickname) != string::npos) {
		cout << CYN ">>> Client FD#" << fd << " est LIMECHAT <<<" NC<< endl;
		m_userDB[fd].m_originNC = FALSE;
	}
	else {
		cout << CYN ">>> Client FD#" << fd << " est probablement NC <<<" NC << endl;
		m_userDB[fd].m_originNC = TRUE;
	}
}

void	Server::printParams(vector<string> &params) {
	int idx = 0;
	cout << "#------------------#" << endl;
	cout << "Printing arguments:" << endl;
	for (vector<string>::const_iterator i = params.begin(); i != params.end(); ++i) {
		cout << "params.at(" << idx << ") == [" << *i << "]" << endl;
		idx++;
	}
	cout << "#------------------#" << endl;
}
vector<Channel>::iterator	Server::channelIsInServer(string channel) {
	vector<Channel>::iterator channelIt = m_allChannels.begin();
	for (; channelIt != m_allChannels.end(); ++channelIt) {
		if ((*channelIt).getChanName().compare(channel) == 0)
			break;
	}
	return (channelIt);
}

int		Server::getFdFromNickname(string nickname) {
	for (int i = 0; m_fds[i].fd; i++) {
        if (m_userDB[m_fds[i].fd].m_nickname == nickname)
            return (m_fds[i].fd);
	}
	return (-1);
}

void	Server::eraseChanFromUserMap(int fd, string chan) {
	map<int, clientInfo>::iterator userIt = m_userDB.find(fd);
	if (userIt != m_userDB.end()) {
		vector<string> &channels = userIt->second.m_channels;
		vector<string>::iterator chanIt = find(channels.begin(), channels.end(), chan);
		if (chanIt != channels.end()) {
		channels.erase(chanIt);
		}
	}
}

void	Server::log(string msg) {
	cerr<< BHGRN "\t[" << getTimestamp() << "] Server Sent: " NC << msg << endl;
}

string Server::getTimestamp() {
    std::time_t now = std::time(NULL);
    char buffer[80];

    std::strftime(buffer, sizeof(buffer), "%H:%M:%S", std::localtime(&now));

    return(buffer);
}

bool	Server::fdIsInServer(int fd) {
	if (fd == -1)
		return (false);
	for (int i = 0; m_fds[i].fd > 0; i++) {
		if (m_fds[i].fd == fd) {
			return (true);
		}
	}
	return (false);
}