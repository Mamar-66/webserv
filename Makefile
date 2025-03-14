NAME	=	Webserv
CC		=	c++
CFLAGS	=	-Wall -Werror -Wextra -Wpedantic -std=c++98 -g

SRC_PATH	=	srcs/
OBJ_PATH	=	objs/

SRC	=	main.cpp \
		serveur/serveur.cpp \
		serveur/routine.cpp \
		clients/client.cpp \
		clients/read.cpp \
		cgi/start_cgi.cpp \
		config/config.cpp \
		config/parssing.cpp \
		RequestHandler/ParserCGI/ParserCGI.cpp \
		RequestHandler/ParserConstructor/ParserConstructors.cpp \
		RequestHandler/ParserErrorChecker/ParserErrorChecker.cpp \
		RequestHandler/ParserResponseMaker/ParserDeleteMaker.cpp \
		RequestHandler/ParserResponseMaker/ParserResponseMaker.cpp \
		RequestHandler/ParserResponseMaker/UtilsParserPostMaker.cpp \
		RequestHandler/ParserResponseMaker/ParserPostMaker.cpp \
		RequestHandler/ParserResponseMaker/UtilsParserResponseMaker.cpp \
		RequestHandler/generalUtils.cpp \
		RequestHandler/ParserGetter.cpp \
		Cookie/Cookies.cpp \
		config/Location.cpp \
		timeout/timeout.cpp \
		utiles/Checkers.cpp \
		utiles/Conversion.cpp \
		utiles/GenericGetter.cpp \
		utiles/Initer.cpp \
		utiles/Modifier.cpp \



SRCS	=	$(addprefix $(SRC_PATH), $(SRC))

OBJS	=	$(SRCS:$(SRC_PATH)%.cpp=$(OBJ_PATH)%.o)

INCS	=	-I ./includes/

CFLAGS_C =	-Wall -Werror -Wextra -g

all: $(OBJ_PATH) $(NAME)

$(OBJ_PATH):
			mkdir -p $(OBJ_PATH)
			mkdir -p $(OBJ_PATH)/RequestHandler
			mkdir -p $(OBJ_PATH)/RequestHandler/ParserCGI
			mkdir -p $(OBJ_PATH)/RequestHandler/ParserConstructor
			mkdir -p $(OBJ_PATH)/RequestHandler/ParserErrorChecker
			mkdir -p $(OBJ_PATH)/RequestHandler/ParserResponseMaker
			mkdir -p $(OBJ_PATH)/serveur
			mkdir -p $(OBJ_PATH)/clients
			mkdir -p $(OBJ_PATH)/config
			mkdir -p $(OBJ_PATH)/cgi
			mkdir -p $(OBJ_PATH)/Cookie
			mkdir -p $(OBJ_PATH)/timeout
			mkdir -p $(OBJ_PATH)/utiles
			mkdir -p json
			mkdir -p uploadServer
			touch json/users.json
			touch json/alreadyUsed.txt
			touch html/conect/commentaire.txt

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
		
supclean: fclean restart

supre: restart re

re: fclean all

init: $(OBJ_PATH)
	@echo "\033[34m\nIniting every file/repertory \033[0m"

restart:
	@echo "\033[34m\nCleaning uploadServer Repertory and removing content of jsons. \033[0m"
	rm -rf uploadServer/*
	rm -rf json/users.json json/alreadyUsed.txt
	touch json/users.json
	@echo "[]" > json/users.json
	touch json/alreadyUsed.txt
	@echo "\033[34m\n Webserv Restarted \033[0m"

.PHONY: all clean fclean re
