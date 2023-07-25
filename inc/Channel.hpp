#pragma once
#include "../inc/IRC.hpp"

// Class declaration
class Channel {
	public:
		Channel(string chanName, int fd);
		Channel(const Channel &other);
		Channel &operator=(const Channel &rhs);
		~Channel();
//------------------------------------ GET -------------------------------------
		int						getNbrMember() const;
		int						getFlagChanPass() const;
		int						getFlagInvitOnly() const;
		int						getFlagOperOnlyTopic() const;
		string					getChanPass() const;
		string					getChanName() const;
		string					getTopic() const;
		vector<int> 			getMemberFdList() const;
//------------------------------------ SET -------------------------------------
		void					setFlagChanPass(int set);
		void					setFlagInvitOnly(int set);
		void					setFlagOperOnlyTopic(int set);
		void					setChanPass(string set);
		void					setTopic(string set);
		void					setMemberLimit(int set);
//----------------------------------- UTILS ------------------------------------
		void					changeNbrMember(string sign);
		void					addFd(int fd);
		void					displayFdList();
		void					displayOperFdList();
		void					eraseFdFromList(int fd);
		void					addKeyChannel(vector<string> &params);
		void					eraseOperFromChannel(int fd);
		void					addOperToChannel(int fd);
		bool					checkKeyChannel(vector<string> &params);
		bool					operatorIsInChannel(int fd);
		bool					fdIsInChannel(int fd);
		bool					isChannelFull();

	private:
		int						m_nbrMember; // Nombre de client dans le channel
		int						m_flagChanPass; // MDP activé pour le channel ? 0/1
		int						m_flagInvitOnly; // Sur invitation activé pour le channel ? 0/1
		int						m_flagOperOnlyTopic; // 1 = seulement oper peut changer le topic 0 = N'importe qui peut changer topic
		int						m_memberLimit;
		string					m_chanName; // Le nom du channel
		string					m_chanPass; // Le mdp du channel
		string					m_topic; // Le topic du channel
		vector<int>				m_memberFdList; //Les fd membres du channel
		vector<int>				m_operatorFdList; //Les fd membres du channel
};

std::ostream& operator<< (std::ostream& out, Channel const &chan);
