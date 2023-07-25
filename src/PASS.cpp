#include "../inc/Server.hpp"

void	Server::checkPASS(vector<string> &params, int fd) {
	if (params.size() > 1) {
		if (isPwdOk(params.at(1)) == FALSE) {
			cout << RED "Invalid Password, connection closed\n" NC << endl;
			m_closeConn = TRUE;

			std::ostringstream oss;
			string message;
			oss << ": 464 " << m_hostname << ": Wrong Password, connection closed\r\n";
			message = oss.str();
			log(message);
			if (send(fd, message.c_str(), message.size(), 0) < 0)
				perror("CMD PASS : message sending failed");
		}

		else {
			cout << MAG "Valid Password, connection accepted\n" NC << endl;
			m_userDB[fd].m_flagWelcome = FALSE;
			m_userDB[fd].m_flagPass = TRUE;
		}
	}
}


bool	Server::isPwdOk(string cppBuffer) {
	if (cppBuffer == m_password)
		return TRUE;
	else
		return FALSE;
}