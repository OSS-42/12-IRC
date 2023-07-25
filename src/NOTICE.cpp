#include "../inc/Server.hpp"

void	Server::commandNOTICE(std::vector<string> &params, int fd) {

	std::ostringstream oss;
	string message;

	for (map<int, clientInfo>::iterator it = m_userDB.begin(); it != m_userDB.end(); ++it) {
		if ((*it).second.m_nickname.compare(params[1]) == 0) {
			oss << ":" << m_userDB[fd].m_nickname << "!" << m_userDB[fd].m_username<< "@" << m_hostname << " NOTICE " << params[1] << " :" << params[2] << "\r\n";
			message = oss.str();
			log(message);
			if (send((*it).first, message.c_str(), message.size(), 0) < 0)
				perror("CMD NOTICE : message sending to user failed");

			return ;
		}
	}

	for (vector<Channel>::iterator chan = m_allChannels.begin(); chan != m_allChannels.end(); ++chan) {
		if ((*chan).getChanName().compare(params[1]) == 0) {
			vector<int> channelMembersFD = (*chan).getMemberFdList();
			for (vector<int>::const_iterator chanFD = channelMembersFD.begin(); chanFD != channelMembersFD.end(); ++chanFD) {
				if ((*chanFD) != fd) {
					oss.str(""); //to clear the stream from previous message.
					oss << ":" << m_userDB[fd].m_nickname << "!" << m_userDB[fd].m_username<< "@" << m_hostname << " NOTICE " << params[1] << " :" << params[2] << "\r\n";
					message = oss.str();
					log(message);
					if (send(*chanFD, message.c_str(), message.size(), 0) < 0)
						perror("CMD NOTICE : message sending to channel failed");
				}
			};
			return ;
		}
	}
}