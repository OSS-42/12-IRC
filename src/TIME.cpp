#include "../inc/Server.hpp"

/*
	Command: TIME
	Parameters: none

   The time message is used to query local time from the server.

   Numeric Replies:

    RPL_TIME

*/

void		Server::commandTIME(vector<string> &params, int fd) {
	string reply;
	(void) params;
	reply = ": 391 IRCServ Localtime: :" + getTimestamp() + "\r\n";
	log(reply);
	if(send(fd, reply.c_str(), reply.size(), 0) < 0)
		cerr << "send() failed" << endl;
	return;	
}