#include "../inc/Server.hpp"

//Command: KICK <channel> <user> [<comment>]

void		Server::commandKICK(vector<string> &params, int fd) {
	string reply;
	if (m_userDB[fd].m_originNC == true && params.size() < 4)
		params.push_back(":");
	if (params.size() > 3) {
		if (m_userDB[fd].m_isOP) { // if caller is an OP, it has the right to kick
			if(params.at(1).empty() || params.at(2).empty()) {
				reply = ": 400 " + params.at(0) + ": Invalid number of arguments. (KICK <channel> <user> [<comment>])\r\n";
				log(reply);
				if(send(fd, reply.c_str(), reply.size(), 0) < 0)
					cerr << "send() failed" << endl;
				return;
			}
			vector<Channel>::iterator chanIt = channelIsInServer(params.at(1));
			if (chanIt == m_allChannels.end()) { // making sure channel exists...
				noSuchChannel(params, fd, chanIt);
				return;
			}
			else if (chanIt->fdIsInChannel(getFdFromNickname(params.at(2))) == true) { // and the fd is currently on the channel.
				vector<string> toKick; // creating a vector with a syntax that works with commandPART()
				toKick.push_back("PART");
				toKick.push_back(params.at(1));
				if (params.size() >= 4 && params.at(3) != ":")
					toKick.push_back("Kicked by " + m_userDB[fd].m_nickname + " | Reason: " + params.at(3));
				else
					toKick.push_back("Kicked by " + m_userDB[fd].m_nickname);
				commandPART(toKick, getFdFromNickname(params.at(2)));
				return ;
			}
			reply = ": 441 " + params.at(2) + " " + params.at(1) + ": User '" + params.at(2) + "' is not on that channel\r\n";
			log(reply);
			if(send(fd, reply.c_str(), reply.size(), 0) < 0)
				cerr << "send() failed" << endl;
			return;
		}
		else { // if caller is NOT an OP
			reply = ": 481 " + m_userDB[fd].m_nickname + ": Permission Denied - You're not an IRC operator\r\n";
			log(reply);
			if(send(fd, reply.c_str(), reply.size(), 0) < 0)
				cerr << "send() failed" << endl;
		}
	}
	else {
		reply = ": 461 " + params.at(0) + ": Invalid number of arguments. (KICK <channel> <user> [<comment>])\r\n";
		log(reply);
		if(send(fd, reply.c_str(), reply.size(), 0) < 0)
			cerr << "send() failed" << endl;
		return;
	}
}