#include "../inc/Server.hpp"

/*
	Command: DIE
	Parameters: None

   An operator can use the DIE command to shutdown the server.
   The DIE command MUST always be fully processed by the server to which
   the sending client is connected and MUST NOT be passed onto other
   connected servers.

   Numeric Replies:
           ERR_NOPRIVILEGES (481)
*/

void		Server::commandDIE(vector<string> &params, int fd) {
	string reply;
	(void)params;
	if (checkPermission(m_userDB[fd].m_nickname) == TRUE) {
		reply = ": 361 " + m_userDB[fd].m_nickname + ": OPER " + m_userDB[fd].m_nickname + " is closing the server. Bye bye. \r\n";
		log(reply);
		for (std::map<int, clientInfo>::iterator it = m_userDB.begin(); it != m_userDB.end(); ++it){
			send(it->first, reply.c_str(), reply.size(), 0);
		}
		m_endServer = TRUE;
	}
	else {
		reply = ": 481 " + m_userDB[fd].m_nickname + ": Permission Denied - You're not an IRC operator\r\n";
		log(reply);
		if(send(fd, reply.c_str(), reply.size(), 0) < 0) {
			cerr << "send() failed" << endl;
		}
		return;
	}
}
