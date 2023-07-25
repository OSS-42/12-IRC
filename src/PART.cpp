#include "../inc/Server.hpp"

/*
	Command: PART <channel> [ <Part Message> ]

   The PART command causes the user sending the message to be removed
   from the list of active members for all given channels listed in the
   parameter string.
   If a "Part Message" is given, this will be sent instead of the default message.
   This request is always granted by the server.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
           ERR_NOTONCHANNEL

   Examples:

   PART #twilight_zone             ; Command to leave channel
                                   "#twilight_zone"

   PART #oz-ops,&group5            ; Command to leave both channels
                                   "&group5" and "#oz-ops".

   :WiZ!jto@tolsun.oulu.fi PART #playzone :I lost
                                   ; User WiZ leaving channel
                                   "#playzone" with the message "I
                                   lost".
*/

void		Server::commandPART(vector<string> &params, int fd) {
	string reply;

	if(params.at(1).empty() || params.at(1) == ":"){
		reply = ": 461 " + params.at(0) + ": Invalid number of arguments. (PART <channel> [<Part Message>])\r\n";
		log(reply);
		if(send(fd, reply.c_str(), reply.size(), 0) < 0)
			cerr << "send() failed" << endl;
		return ;
	}
	else {
		vector<Channel>::iterator chanIt = channelIsInServer(params.at(1));
		if (chanIt == m_allChannels.end()) {
			reply = ": 403 " + params.at(1) + ": No such channel. Maybe you forgot the # ?\r\n";
			log(reply);
			if(send(fd, reply.c_str(), reply.size(), 0) < 0)
				cerr << "send() failed" << endl;
			return;
		}
		else if (chanIt->fdIsInChannel(fd) == true) {
			reply = ":" + m_userDB[fd].m_nickname + "!" + m_userDB[fd].m_username + "@" + m_hostname + " PART " + params.at(1);
				if (params.size() == 3)
					reply += " : " + params.at(2) + " \r\n";
				else
					reply +=  + "\r\n";
			log(reply);
			chanIt->eraseFdFromList(fd);
			eraseChanFromUserMap(fd, params.at(1));
			if(send(fd, reply.c_str(), reply.size(), 0) < 0)
				cerr << "send() failed" << endl;
			return ;
		}
		else
			notInChannel(params, fd);
	}
}