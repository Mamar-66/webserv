NAME	=	Webserv
CGI_tester	=	tester
CC		=	c++
CFLAGS	=	-Wall -Werror -Wextra -Wpedantic -std=c++98 -g

SRC_PATH	=	srcs/
OBJ_PATH	=	objs/

SRC	=	main.cpp \
		serveur/serveur.cpp \
		serveur/routine.cpp \
		serveur/creat_servors.cpp \
		clients/client.cpp \
		cgi/start_cgi.cpp \
		config/config.cpp \
		config/parssing.cpp \
		RequestHandler/ParserCGI/ParserCGI.cpp \
		RequestHandler/ParserCGI/UtilsParserCGI.cpp \
		RequestHandler/ParserConstructor/ParserConstructors.cpp \
		RequestHandler/ParserErrorChecker/ParserErrorChecker.cpp \
		RequestHandler/ParserErrorChecker/UtilsParserErrorChecker.cpp \
		RequestHandler/ParserResponseMaker/ParserDeleteMaker.cpp \
		RequestHandler/ParserResponseMaker/UtilsParserDeleteMaker.cpp \
		RequestHandler/ParserResponseMaker/ParserResponseMaker.cpp \
		RequestHandler/ParserResponseMaker/UtilsParserPostMaker.cpp \
		RequestHandler/ParserResponseMaker/ParserPostMaker.cpp \
		RequestHandler/ParserResponseMaker/UtilsParserResponseMaker.cpp \
		RequestHandler/generalUtils.cpp \
		RequestHandler/ParserGetter.cpp \
		Cookie/Cookies.cpp \
		Cookie/UtilsCookie.cpp \
		config/Location.cpp \
		utiles/Conversion.cpp


SRCS	=	$(addprefix $(SRC_PATH), $(SRC))

OBJS	=	$(SRCS:$(SRC_PATH)%.cpp=$(OBJ_PATH)%.o)
OBJS_CGI	=	$(SRCS:cgi/testeur.cpp=$(OBJ_PATH)%.o)

INCS	=	-I ./includes/

CFLAGS_C =	-Wall -Werror -Wextra -g

all: $(OBJ_PATH) $(NAME) $(CGI_tester)

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
			mkdir -p json
			mkdir -p uploadServer
			touch json/users.json
			touch json/alreadyUsed.txt

$(NAME): $(OBJS)
	@echo "\033[0;33m\nCOMPILING \033[31m Webserv \033[0;33m ...\n"
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "\033[32m ./Webserv created\n"

$(CGI_tester): $(OBJS_CGI)
	@echo "\033[0;33m\nCOMPILING \033[31m CGI_tester \033[0;33m ...\n"
	c++ srcs/cgi/testeur.cpp -o $(CGI_tester)
	@echo "\033[32m ./tester created\n"

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(INCS)

clean:
		rm -rf $(OBJ_PATH)

fclean: clean
		rm -rf $(NAME)
		rm -rf $(CGI_tester)

re: fclean all

.PHONY: all clean fclean re

