NAME	=	Webserv
CC		=	c++
CFLAGS	=	-Wall -Werror -Wextra -Wpedantic -std=c++98 -g

SRC_PATH	=	srcs/
OBJ_PATH	=	objs/

SRC	=	main.cpp \
		utiles/utiles.cpp \
		serveur/serveur.cpp \
		serveur/routine.cpp \
		serveur/creat_servors.cpp \
		clients/client.cpp \
		config/config.cpp \
		config/parssing.cpp 


SRCS	=	$(addprefix $(SRC_PATH), $(SRC))

OBJS	=	$(SRCS:$(SRC_PATH)%.cpp=$(OBJ_PATH)%.o)

INCS	=	-I ./includes/

CFLAGS_C =	-Wall -Werror -Wextra -g

all: $(OBJ_PATH) $(NAME)

$(OBJ_PATH):
			mkdir -p $(OBJ_PATH)
			mkdir -p $(OBJ_PATH)/serveur
			mkdir -p $(OBJ_PATH)/clients
			mkdir -p $(OBJ_PATH)/utiles
			mkdir -p $(OBJ_PATH)/config

$(NAME): $(OBJS)
	@echo "\033[0;33m\nCOMPILING \033[31m Webserv \033[0;33m ...\n"
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "\033[32m ./Webserv created\n"

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(INCS)

clean:
		rm -rf $(OBJ_PATH)

fclean: clean
		rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
