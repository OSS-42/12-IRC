#include "../inc/Channel.hpp"

// Default constructor
Channel::Channel(string chanName, int fd) : m_nbrMember(1), m_flagChanPass(FALSE), m_flagInvitOnly(FALSE), m_flagOperOnlyTopic(TRUE), m_memberLimit(0), m_chanName(chanName), m_chanPass(""), m_topic(""){
    m_memberFdList.push_back(fd);
    return ;
}

// Copy constructor
Channel::Channel(const Channel &other) {
	*this = other;
	return;
}

// Copy assignment overload
Channel &Channel::operator=(const Channel &rhs) {
	if (this != &rhs) {
		m_nbrMember = rhs.m_nbrMember;
		m_flagChanPass = rhs.m_flagChanPass;
		m_chanPass = rhs.m_chanPass;
		m_chanName = rhs.m_chanName;
		m_topic = rhs.m_topic;
		m_memberFdList = rhs.m_memberFdList;
	}
	return (*this);
}

// Default destructor
Channel::~Channel() { return; }


// ------------------------------------ GET ------------------------------------
int		Channel::getNbrMember() const {
	return(m_nbrMember);
}

int		Channel::getFlagChanPass() const {
	return(m_flagChanPass);
}

int		Channel::getFlagInvitOnly() const {
	return(m_flagInvitOnly);
}

int		Channel::getFlagOperOnlyTopic() const {
	return (m_flagOperOnlyTopic);
}

string	Channel::getChanPass() const {
		return(m_chanPass);
}

string	Channel::getChanName() const {
	return(m_chanName);
}

string	Channel::getTopic() const {
	return(m_topic);
}

vector<int> Channel::getMemberFdList() const {
		return m_memberFdList;
}

// ------------------------------------ SET ------------------------------------
void	Channel::setFlagChanPass(int set) {
	this->m_flagChanPass = set;
}

void	Channel::setChanPass(string set) {
	if (set == "\0")
		this->m_chanPass.clear();
	else
		this->m_chanPass = set;
}

void	Channel::setFlagOperOnlyTopic(int set) {
	this->m_flagOperOnlyTopic = set;
}

void	Channel::setTopic(string set) {
	this->m_topic = set;
}

void	Channel::setMemberLimit(int set) {
	m_memberLimit = set;
}

void	Channel::setFlagInvitOnly(int set) {
	m_flagInvitOnly = set;
}

// ---------------------------------- UTILIS -----------------------------------

void	Channel::changeNbrMember(string sign) {
		if(sign.compare("+") == 0)
				m_nbrMember++;
		else if(sign.compare("-") == 0)
				m_nbrMember--;
		else
			cerr << RED "Bad argument in ChangeNbrMember()" NC << endl;
}

void	Channel::addFd(int fd) {
		m_memberFdList.push_back(fd);
}

void	Channel::displayFdList() {
		for (vector<int>::iterator it = m_memberFdList.begin(); it != m_memberFdList.end(); it++) {
				cout << YEL "- " <<  *it << NC << endl;
		}
}

void	Channel::displayOperFdList() {
		for (vector<int>::iterator it = m_operatorFdList.begin(); it != m_operatorFdList.end(); it++) {
				cout << YEL "- " <<  *it << NC << endl;
		}
}

bool  Channel::fdIsInChannel(int fd) {
		vector<int>::iterator it = m_memberFdList.begin();
		for (; it != m_memberFdList.end(); it++) {
				if ((*it) == fd)
					break;
		}
		if (it != m_memberFdList.end())
			return (true); //Si je ne suis pas à la fin alors l'utilisateur est déjà dans le channel
		return (false);
}

bool  Channel::operatorIsInChannel(int fd) {
		vector<int>::iterator it = m_operatorFdList.begin();
		for (; it != m_operatorFdList.end(); it++) {
				if ((*it) == fd)
					break;
		}
		if (it != m_operatorFdList.end())
			return (true); //Si je ne suis pas à la fin alors l'operateur est déjà dans le channel
		return (false);
}

void    Channel::eraseFdFromList(int fd) {
		for (vector<int>::iterator fdListIt = m_memberFdList.begin(); fdListIt != m_memberFdList.end(); fdListIt++)
			if ((*fdListIt) == fd) {
					m_memberFdList.erase(fdListIt);
					changeNbrMember("-");
					break;
			}
}

void    Channel::addKeyChannel(vector<string> &params) {
		if (params[2].find(" ") != string::npos)
				params[2] = params[2].substr(1);
		m_flagChanPass = TRUE;
		if (params.size() > 3) {
				string mergedParams;
				for (size_t i = 2; i < params.size(); i++) {
						mergedParams += params[i];
						if (i < params.size() - 1)
								mergedParams += " "; // Ajouter un espace entre les mots fusionnés
				}
				m_chanPass = mergedParams;
		}
		else
				m_chanPass = params[2];
}

bool    Channel::checkKeyChannel(vector<string> &params) {
		if (params[2].find(" ") != string::npos)
				params[2] = params[2].substr(1);
		if (m_chanPass.compare(params[2]) == 0)
				return (true);
		return (false);
}



void	Channel::eraseOperFromChannel(int fd) {
	for (vector<int>::iterator it = m_operatorFdList.begin(); it != m_operatorFdList.end(); it++)
		if ((*it) == fd) {
			m_operatorFdList.erase(it);
			return ;
		}
}

void	Channel::addOperToChannel(int fd) {
	vector<int>::iterator it = m_operatorFdList.begin();
	for (; it != m_operatorFdList.end(); it++)
		if ((*it) == fd)
			break;
	if (it == m_operatorFdList.end())
		m_operatorFdList.push_back(fd);
}

bool	Channel::isChannelFull() {
	if (m_memberLimit > 0) {
		if (m_nbrMember == m_memberLimit)
			return (true);
	}
	return (false);
}

std::ostream& operator<< (std::ostream& out, const Channel& chan) {
	out << "ChanName: " << chan.getChanName();
	out << "\nChanPass: " << chan.getChanPass();
	out << "\nPassFlag: " << chan.getFlagChanPass();
	out << "\nMembers: " << chan.getNbrMember();
	out << "\nTopic: " << chan.getTopic();
	out << "\nTotal nb of channels: " << std::flush;
	return (out);
}