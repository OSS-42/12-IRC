#include "../inc/Server.hpp"

// TOPIC #channel new_topic : défini un nouveau topic si l'utilisateur à les droits
// TOPIC #channel : affiche le topic du channel
void	Server::commandTOPIC(vector<string> &params, int fd) {
		if (params.size() < 2)
			notEnoughtParams(params, fd);
		else {
			vector<Channel>::iterator chanIt = channelIsInServer(params.at(1));
			if (chanIt == m_allChannels.end())
				noSuchChannel(params, fd, chanIt);
			else if (chanIt->fdIsInChannel(fd) == false)
				notInChannel(params, fd);
			else if (params.size() > 2)
			{
				if (chanIt->getFlagOperOnlyTopic() == TRUE && m_userDB[fd].m_isOP == false && chanIt->operatorIsInChannel(fd) == false)
					needOperPriv(params, fd);
				else
					newTopicProcess(params, fd, chanIt);
			}
			else
				displayTopicProcess(params, fd, chanIt);
	}
}

void	Server::newTopicProcess(vector<string> &params, int fd, vector<Channel>::iterator channel) {
	string topic = params[2];
	for (vector<string>::iterator topicIt = params.begin() + 3; topicIt != params.end(); ++topicIt)
		topic += " " + (*topicIt);
	string successMessage = ":" + m_hostname + " " + std::to_string(RPL_CHANNELMODEIS) + " " + m_userDB[fd].m_nickname + " " + params[1] + " +t. The new topic is: " + topic + ".\r\n";
	log(successMessage);
	if (send(fd, successMessage.c_str(), successMessage.size(), 0) < 0)
		cerr << "send() failed" << endl;
	else
		channel->setTopic(topic);
}

void	Server::displayTopicProcess(vector<string> &params, int fd, vector<Channel>::iterator channel) {
	string buffer = ":" + m_hostname + " " + std::to_string(RPL_TOPIC) + " " + m_userDB[fd].m_nickname + " " + params[1] + " " + channel->getTopic() + "\r\n";
	log(buffer);
	if (send(fd, buffer.c_str(), buffer.size(), 0) < 0)
		cerr << "send() failed" << endl;
}
