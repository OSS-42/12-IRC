#pragma once
#include "../inc/IRC.hpp"
#include "../inc/Channel.hpp"
#include "../inc/numeric_replies.hpp"

typedef void (*cmdFunPtr)(vector<string> &params, int fd);

class Channel;
class Server
{
	public:
		Server(string pass);
		~Server();

//	---------------------------------- SERVER ----------------------------------
		void		initSocket();
		void		setSocket();
		void		bindSocket();
		void		listenSocket();
		void		processSocket();
		void		acceptReceive();
		void		acceptSocket();
		void		receiveSocket(int &i);
		void		closeConnection(int &i);
		void		compressArray();
		void		closeServer();

//	---------------------------------- PARSING ---------------------------------
		void		checkPASS(vector<string> &params, int fd);
		void		splitBuffer(char* buffer);
		void		execCommand(vector<string> &params, int fd);
		void		clientDetection(int fd, string registration);

		bool		isPwdOk(string cppBuffer);
		void		storeBufferValue(vector<string> &params, int fd);
		void		welcomeMessage(int fd);

//	----------------------------------- NICK -----------------------------------
		void		commandNICK(vector<string> &params, int fd);
		bool		isNickTaken(string nickname);
		bool		isNickOK(string nickname);

//	----------------------------------- JOIN -----------------------------------
		void		commandJOIN(vector<string> &params, int fd);
		void		joinProcess(vector<string> &params, int fd, vector<Channel>::iterator channelIt);
		bool		isInChannel(vector<Channel>::iterator it, int fd);
		void		firstChannel(vector<string> &params, int fd, Channel channel, string buffer);
		void		newChannel(vector<string> &params, int fd, Channel channel, string buffer);
		void		addUserToChannel(vector<string> &params, int fd, vector<Channel>::iterator channelIt, string buffer);

//	---------------------------------- PRIVMSG ---------------------------------
		void		commandPRIVMSG(vector<string> &params, int fd);

//	---------------------------------- NOTICE ----------------------------------
		void		commandNOTICE(vector<string> &params, int fd);

//	----------------------------------- OPER -----------------------------------
		void 		commandOPER(vector<string> &params, int fd);
		int 		checkPermission(string funCaller) const;
		void		setOP(int fd);

//	----------------------------------- PART -----------------------------------
		void		commandPART(vector<string> &params, int fd);

//	----------------------------------- KICK -----------------------------------
		void		commandKICK(vector<string> &params, int fd);

//	----------------------------------- INVITE -----------------------------------
		void		commandINVITE(vector<string> &params, int fd);
		void		inviteProcess(vector<string> &params, int fd, vector<Channel>::iterator channel);

//	----------------------------------- INVITE -----------------------------------
		void		commandTOPIC(vector<string> &params, int fd);
		void		newTopicProcess(vector<string> &params, int fd, vector<Channel>::iterator channel);
		void		displayTopicProcess(vector<string> &params, int fd, vector<Channel>::iterator channel);

//	----------------------------------- TIME -----------------------------------
		void		commandTIME(vector<string> &params, int fd);

//	 ----------------------------------- DIE -----------------------------------
		void		commandDIE(vector<string> &params, int fd);

//	----------------------------------- MODE -----------------------------------
		void		commandMODE(vector<string> &params, int fd);
		void		modeProcess(vector<string> &params, int fd);

		// ------------------------- MODE L -------------------------
		void		modeL(vector<string> &params, int fd);
		void		positiveL(vector<string> &params, int fd, vector<Channel>::iterator channel);
		void		negativeL(vector<string> &params, int fd, vector<Channel>::iterator channel);

		// ------------------------- MODE I -------------------------
		void		modeI(vector<string> &params, int fd);
		void		positiveI(vector<string> &params, int fd, vector<Channel>::iterator channel);
		void		negativeI(vector<string> &params, int fd, vector<Channel>::iterator channel);

		// ------------------------- MODE T -------------------------
		void		modeT(vector<string> &params, int fd);
		void		positiveT(vector<string> &params, int fd, vector<Channel>::iterator channel);
		void		negativeT(vector<string> &params, int fd, vector<Channel>::iterator channel);

		// ------------------------- MODE K -------------------------
		void		modeK(vector<string> &params, int fd);
		void		positiveK(vector<string> &params, int fd, vector<Channel>::iterator channel);
		void		negativeK(vector<string> &params, int fd, vector<Channel>::iterator channel);

		// ------------------------- MODE O -------------------------
		void		modeO(vector<string> &params, int fd);
		void		positiveO(vector<string> &params, int fd, vector<Channel>::iterator channel);
		void		negativeO(vector<string> &params, int fd, vector<Channel>::iterator channel);

//	----------------------------------- UTILS ----------------------------------
		void						log(string msg);
		int     					getChanCount() const;
		static void					signalHandler(int signal_num);
		void						printAllChannel();
		void						printParams(vector<string> &params);
		vector<Channel>::iterator	channelIsInServer(string channel);
		int							getFdFromNickname(string nickname);
		void						eraseChanFromUserMap(int fd, string chan);
		string 						getTimestamp();
		bool						fdIsInServer(int fd);

//	----------------------------- ERROR MANAGEMENT ----------------------------
		void	notEnoughtParams(vector<string> &params, int fd);
		void	noSuchChannel(vector<string> &params, int fd, vector<Channel>::iterator channel);
		void	notInChannel(vector<string> &params, int fd);
		void	needOperPriv(vector<string> &params, int fd);
		void	targetNotInChannel(vector<string> &params, int fd, vector<Channel>::iterator channel);
		void	badChannelKey(vector<string> &params, int fd);
		void	channelIsFull(vector<string> &params, int fd, vector<Channel>::iterator channelIt);
		void	badNumericParam(vector<string> &params, int fd);
		void	inviteOnlyChan(vector<string> &params, int fd, vector<Channel>::iterator channelIt);
		void	noSuchNick(vector<string> &params, int fd);
		void	userOnChannel(vector<string> &params, int fd);

	private:
		int							m_socket;
		int							m_rv; //return value
		int							m_on;
		int							m_port;
		int							m_nbFds;
		int							m_newSD;
		int							m_endServer;
		int							m_closeConn;
		int							m_compressArray;
		string						m_hostname;
		const string				m_password;
		string						m_tmpBuff;
		bool						m_partialReq;
		char						m_buffer[512];
		struct sockaddr_in6			m_addr;
		struct pollfd				m_fds[OPEN_MAX];

//	----------------------------------- USER ----------------------------------
		struct	clientInfo {
			string					m_nickname;
			string					m_username;
			bool					m_flagWelcome;
			bool					m_flagPass;
			bool					m_originNC;
			vector<string>			m_channels;
			bool					m_isOP;
		};

		map<int, clientInfo>		m_userDB;
		vector<string>				m_permissionList;
		vector<string>				m_args;
		vector<Channel>				m_allChannels;
};

extern Server* gServer;