#include "../inc/Server.hpp"

int Server::checkPermission(string funCaller) const {
	for (vector<string>::const_iterator idP = m_permissionList.begin(); idP != m_permissionList.end(); ++idP) {
		if (*idP == funCaller)
			return (TRUE);
	}
	return (FALSE);
}

void	Server::setOP(int fd) {
	string	reply;
	if (m_userDB[fd].m_isOP != TRUE) {
		m_userDB[fd].m_isOP = TRUE;
		reply = ": 381 " + m_userDB[fd].m_nickname + ": You are now an IRC operator\r\n";
		log(reply);
		if(send(fd, reply.c_str(), reply.size(), 0) < 0)
			cerr << "send() failed" << endl;
		return;
	}
	reply = ": 400 " + m_userDB[fd].m_nickname + ": You are an IRC operator already\r\n";
	log(reply);
	if(send(fd, reply.c_str(), reply.size(), 0) < 0)
		cerr << "send() failed" << endl;
	return;
}


void	Server::commandOPER(vector<string> &params, int fd)
{
	string	reply;

	if (params.size() != 3) {
		reply = ": 461 " + params.at(0) + ": Invalid numbers of arguments. (OPER <name> <pass>)\r\n";
		log(reply);
		if(send(fd, reply.c_str(), reply.size(), 0) < 0)
			cerr << "send() failed" << endl;
		return ;
	}

	string	inputName = params.at(1);

	if (params.at(2).compare("4242") != 0) { // checks for a valid password
		reply = ": 464 " + m_userDB[fd].m_nickname + ": Password incorrect\r\n";
		log(reply);
		if(send(fd, reply.c_str(), reply.size(), 0) < 0)
			cerr << "send() failed" << endl;
		return ;
	}

	else {
		if (checkPermission(m_userDB[fd].m_username) == TRUE) { // if the person calling OPER is part of permissionList
			if (m_userDB[fd].m_username.compare(inputName) == 0) { // caller is setting their own OP flag
				setOP(fd);
				return ;
			}
			if (m_userDB[fd].m_username.compare(inputName) != 0) { // caller is setting another user's OP flag
				if (m_userDB[fd].m_isOP == TRUE) {
					for (map<int, clientInfo>::iterator idU = m_userDB.begin(); idU != m_userDB.end(); ++idU)
					{
						if (inputName.compare(idU->second.m_username) == 0) { // checking if the other user exists
							setOP(idU->first);
							return ;
						}
						else
							continue ;
					}
				}
			else {
				reply = ": 481 " + m_userDB[fd].m_nickname + ": Permission Denied - You're not an IRC operator\r\n";
				log(reply);
				if(send(fd, reply.c_str(), reply.size(), 0) < 0)
					cerr << "send() failed" << endl;
		}

			}
		}
		else {
			reply = ": 481 " + m_userDB[fd].m_nickname + ": Permission Denied - You're not an IRC operator\r\n";
			log(reply);
			if(send(fd, reply.c_str(), reply.size(), 0) < 0)
				cerr << "send() failed" << endl;
		}
	}
}
