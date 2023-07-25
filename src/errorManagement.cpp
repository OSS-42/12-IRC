#include "../inc/Server.hpp"

void	Server::notEnoughtParams(vector<string> &params, int fd) {
	string errorMessage = ":" + m_hostname + " " + std::to_string(ERR_NEEDMOREPARAMS) + " " + m_userDB[fd].m_nickname + " " + params[0] + " :Not enough parameters\r\n";
	log(errorMessage);
	if (send(fd, errorMessage.c_str(), errorMessage.size(), 0) < 0)
		cerr << "send() failed" << endl;
}

void	Server::noSuchChannel(vector<string> &params, int fd, vector<Channel>::iterator channel) {
	(void)params;
	(void)channel;
	string errorMessage = ": " + std::to_string(ERR_NOSUCHCHANNEL) + " " + m_userDB[fd].m_nickname + " :No such channel. Maybe you forgot the # ?\r\n";
	log(errorMessage);
	if (send(fd, errorMessage.c_str(), errorMessage.size(), 0) < 0)
  		cerr << "send() failed" << endl;
}

void	Server::notInChannel(vector<string> &params, int fd) { //Si l'utilisateur de la commande n'est pas dans le channel
	(void)params;
	string errorMessage = ": " + std::to_string(ERR_NOTREGISTERED) + " " + m_userDB[fd].m_nickname + " :You need to be register in the channel\r\n";
	log(errorMessage);
	if (send(fd, errorMessage.c_str(), errorMessage.size(), 0) < 0)
  		cerr << "send() failed" << endl;
}

void	Server::targetNotInChannel(vector<string> &params, int fd, vector<Channel>::iterator channel) { //Si la CIBLE de la commande de l'utilisateur n'est pas dans le channel
	string errorMessage = ":" + m_hostname + " " + std::to_string(ERR_USERNOTINCHANNEL) + " " + params[3] + " " + channel->getChanName() + " :" + params[3] + " is not on that channel\r\n";
	log(errorMessage);
	if (send(fd, errorMessage.c_str(), errorMessage.size(), 0) < 0)
		cerr << "send() failed" << endl;
}

void	Server::needOperPriv(vector<string> &params, int fd) {
	string errorMessage = ": " + std::to_string(ERR_CHANOPRIVSNEEDED) + " " + m_userDB[fd].m_nickname + " " + params[2] + " :User need operator privilege\r\n";
	log(errorMessage);
	if (send(fd, errorMessage.c_str(), errorMessage.size(), 0) < 0)
  		cerr << "send() failed" << endl;
}

void	Server::badChannelKey(vector<string> &params, int fd) {
	string errorMessage = ":" + m_hostname + " " + std::to_string(ERR_BADCHANNELKEY) + " " + params[1] + " :Bad Channel Key. Maybe you forgot the : at the begining of the key ?\r\n";
	log(errorMessage);
	if (send(fd, errorMessage.c_str(), errorMessage.size(), 0) < 0)
		cerr << "send() failed" << endl;
}

void	Server::channelIsFull(vector<string> &params, int fd, vector<Channel>::iterator channelIt) {
	string errorMessage = ":" + m_hostname + " " + std::to_string(ERR_CHANNELISFULL) + " " + params[1] + channelIt->getChanName() + " :Cannot join channel because it's full\r\n";
	log(errorMessage);
	if (send(fd, errorMessage.c_str(), errorMessage.size(), 0) < 0)
		cerr << "send() failed" << endl;
}

void	Server::badNumericParam(vector<string> &params, int fd) {
	string errorMessage = ":" + m_hostname + " " + std::to_string(ERR_CHANNELISFULL) + " " + params[1] + " :Bad numeric parameter. The value must be bigger than 0\r\n";
	log(errorMessage);
	if (send(fd, errorMessage.c_str(), errorMessage.size(), 0) < 0)
		cerr << "send() failed" << endl;
}

void	Server::inviteOnlyChan(vector<string> &params, int fd, vector<Channel>::iterator channelIt) {
	string errorMessage = ":" + m_hostname + " " + std::to_string(ERR_INVITEONLYCHAN) + " " + params[1] + " :Cannot join the channel " + channelIt->getChanName() + ". An invitation is required.\r\n";
	log(errorMessage);
	if (send(fd, errorMessage.c_str(), errorMessage.size(), 0) < 0)
		cerr << "send() failed" << endl;
}

void	Server::noSuchNick(vector<string> &params, int fd) {
	string errorMessage = ":" + m_hostname + " " + std::to_string(ERR_NOSUCHNICK) + " " + params[1] + " :User doesn't exist\r\n";
	log(errorMessage);
	if (send(fd, errorMessage.c_str(), errorMessage.size(), 0) < 0)
		cerr << "send() failed" << endl;
}

void	Server::userOnChannel(vector<string> &params, int fd) {
	string errorMessage = ":" + m_hostname + " " + std::to_string(ERR_USERONCHANNEL) + " " + params[1] + " :User is already in the channel\r\n";
	log(errorMessage);
	if (send(fd, errorMessage.c_str(), errorMessage.size(), 0) < 0)
		cerr << "send() failed" << endl;
}