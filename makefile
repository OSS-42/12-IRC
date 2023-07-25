#------------------------------------------------------------------------------#
#                                  GENERICS                                    #
#------------------------------------------------------------------------------#

# Special variables
DEFAULT_GOAL: all
.DELETE_ON_ERROR: $(NAME)
.PHONY: all bonus clean fclean re

# Hide calls
export VERBOSE	=	TRUE
ifeq ($(VERBOSE),TRUE)
	HIDE =
else
	HIDE = @
endif

#------------------------------------------------------------------------------#
#                                VARIABLES                                     #
#------------------------------------------------------------------------------#

# Compiler and flags
CC		=	c++
CFLAGS	=	-Wall -Werror -Wextra -std=c++98 -I. -g
#CFLAGS	=	-Wall -Wextra -Werror -std=c++98 -I.
RM		=	rm -rf

# Dir and file names

NAME	=	ircserv
# SRCS	=	$(addprefix src/, main.cpp Client.cpp Server.cpp)
# OBJS	=	$(SRCS:.cpp=.o)
# INC		=	$(addprefix inc/, wildcard *.hpp)

D_INC	=	inc/
D_SRC	=	src/
D_OBJ	=	obj/
OBJS	=	$(patsubst $(D_SRC)%.cpp,$(D_OBJ)%.o,$(SRCS))
INCS	=	inc/IRC.hpp \
			inc/Server.hpp \
			inc/Channel.hpp \
			inc/numeric_replies.hpp

SRCS	=	src/main.cpp \
			src/Channel.cpp \
			src/JOIN.cpp \
			src/NICK.cpp \
			src/PASS.cpp \
			src/PRIVMSG.cpp \
			src/NOTICE.cpp \
			src/ServerInit.cpp \
			src/ServerProcess.cpp \
			src/ServerUtils.cpp\
			src/signal.cpp \
			src/OPER.cpp	\
			src/KICK.cpp	\
			src/MODE.cpp	\
			src/PART.cpp 	\
			src/DIE.cpp		\
			src/TIME.cpp	\
			src/INVITE.cpp 	\
			src/TOPIC.cpp 	\
			src/errorManagement.cpp

# À VOIR SI NOTRE SEPARATION DES FONCTIONS DU SERVER EST VALIDE SELON LA NORME #

#------------------------------------------------------------------------------#
#                                 TARGETS                                      #
#------------------------------------------------------------------------------#

all: $(NAME)

# Generates output file
$(NAME):	$(D_OBJ) $(OBJS)
	$(HIDE)$(CC) $(CFLAGS) $(OBJS) -o $@

$(D_OBJ):
	@mkdir -p $(D_OBJ)

# Compiles sources into objects
# .cpp.o: $(SRCS) $(INC)
# 	$(HIDE)$(CC) $(CFLAGS) -c $< -o $@

$(OBJS): $(D_OBJ)%.o : $(D_SRC)%.cpp $(INC)
	$(HIDE)$(CC) $(CFLAGS) -c $< -o $@

# Removes objects
clean:
	$(HIDE)$(RM) $(D_OBJ)

# Removes objects and executables
fclean: clean
	$(HIDE)$(RM) $(NAME)
	$(RM) *.dSYM

# Removes objects and executables and remakes
re: fclean all
