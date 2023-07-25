#pragma once

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <csignal>

#include <iostream>
#include <sstream>
#include <string>

#include <map>
#include <vector>
#include <list>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::map;

#define TRUE 1
#define FALSE 0
//#define TIMEOUT 15 * 1000
#define TIMEOUT 3 * 60 * 1000

// COLOURS CODES
# define NC "\e[0m"
# define RED "\e[0;31m"
# define GRN "\e[0;32m"
# define YEL "\e[0;33m"
# define MAG "\e[0;35m"
# define CYN "\e[0;36m"
# define REDB "\e[41m"
# define BHGRN "\e[1;92m"

#define DEBUG

#ifdef DEBUG
#define DEBUG_MSG(str1, str2) do { cerr << "\t[DEBUG] " << str1 << " [" << str2 << "]" << endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif
