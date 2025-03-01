#include "../../includes/StaticClasses/Checkers.hpp"
#include "../../includes/StaticClasses/GenericGetter.hpp"

Checker::Checker() {}
Checker::~Checker() {}
Checker::Checker(Checker& cpy) {
    (void)cpy;
}
Checker& Checker::operator=(Checker& cpy) {
    (void)cpy;
    return *this;
}

bool Checker::isDirectory(const std::string& path) {
    struct stat pathStat;
    if (stat(path.c_str(), &pathStat) == 0) {
        // Vérifie si le type correspond à un répertoire
        return (pathStat.st_mode & S_IFMT) == S_IFDIR;
    }
    return false; // Chemin invalide ou erreur
}

bool Checker::isFile(const std::string& path) {
    struct stat pathStat;
    if (stat(path.c_str(), &pathStat) == 0) {
        // Vérifie si le type correspond à un fichier régulier
        return (pathStat.st_mode & S_IFMT) == S_IFREG;
    }
    return false; // Chemin invalide ou erreur
}

int Checker::compNb(std::string s) {
    for (int i = 0; i < static_cast<int>(s.size()); i++) {
        if ((s[i] < '0' || s[i] > '9') && s[i] != '\r' && s[i] != '\n' && s[i] != ' ')
            return 0;
    }
    return (1);
}

int Checker::AnalyseStacks(std::stack<char> elems, std::stack<MyTriple<char, char, char> > comas)
{
    int count1 = 0;
    int count2 = 0;
    char c;
    MyTriple<char, char, char> triple;

    /* Check coherence {} [] */
    while (! elems.empty() ) {
        c = elems.top();
        elems.pop();
        if (c == '[')
            count1++;
        else if (c == '{')
            count2++;
        else if (c == '}')
            count2--;
        else if (c == ']')
            count1--;
            
        if (count1 > 0 || count2 > 0)
            return 400;
    }
    if (count1 != 0 || count2 != 0)
        return 400;


    /* Check coherence comas */
    while (! comas.empty()) {
        triple = comas.top();
        comas.pop();

        if (triple.first == '[' || triple.first == '{' || triple.third == ']' || triple.third == '}')
            return 400;
    }
    return 200;
}

int Checker::analyseSub(std::string body, int i) {
    
    if (GenericGetter::GenericGetter::removeSpaces(body, BEFORE, i) != '"')
        return (0);
    i++;
    while (body[i] == ' ') 
        i++;

    if (body[i] == '"')
        return (1);
    else {
        while (body[i] != ' ' && body[i] ) {
            if (body[i] < '0' || body[i] > '9')
                return (0);
            i++;
        }
        return (1);
    }
}

int Checker::parseJson(std::string& body) {
    std::stack<char> stackElem;
    std::stack<MyTriple<char, char, char> > stackComa;
    std::stack<MyTriple<char, char, std::string> > stackKeyValue;

    if (body[0] != '[' && body[0] != '{')
        return 400;
    stackElem.push(body[0]);
    for (int i = 1; i < static_cast<int>(body.size()) - 1; i++) {
        if (body[i] == '"') {
            i++;
            while (body[i] != '"' && body[i] != '\0')
                i++;
            if (body[i] != '"')
                return 400;
            i++;
        }
        if (body[i] == '[' || body[i] == '{')
            stackElem.push(body[0]);

        if (body[i] == ',')
            stackComa.push(MyTriple<char, char, char>(GenericGetter::removeSpaces(body, BEFORE, i), body[i], GenericGetter::removeSpaces(body, AFTER, i)));
        if (body[i] == ':' && !(Checker::analyseSub(body, i)))
            return 400;
    }
    stackElem.push(body[body.size() - 1]);

    return Checker::AnalyseStacks(stackElem, stackComa);
}

int Checker::parseCatFile(std::string f, RequestIn& req) { //, ParseConfig& conf) {
    std::istringstream iss(f);
    std::string root = GenericGetter::findRoot(req);
    std::istringstream is2(root); // Change to conf.getRoot()

    MyStack<std::string> stackRepo;
    MyStack<std::string> stackDefault;

    std::string s;
    while ( std::getline( iss, s, '/' ) ) {
        if (s == ".." && stackRepo.empty())
            return 0; 
        if (s == "..") 
            stackRepo.pop();
        else if (s == "." && stackRepo.empty())
            stackRepo.push(s);
        else if (s == ".") 
            ;
        else 
            stackRepo.push(s);
    }

    while ( std::getline( is2, s, '/' ) ) {
        if (s == ".." && stackDefault.empty())
            return 0; 
        else if (s == "..")
            stackDefault.pop();
        else if (s == "." && stackDefault.empty())
            stackDefault.push(s);
        else if (s == ".") 
            ;
        else 
            stackDefault.push(s);
    }
    stackRepo.reverse();
    stackDefault.reverse();
    while (!(stackDefault.empty())) {
        if (stackDefault.popTop() != stackRepo.popTop())
            return 0;
    }
    return 1;
}

bool	Checker::if_close_pipe(monitoring &moni, int fd, int i)
{
	if (moni.all_all_pollfd[i].revents & POLLHUP)
	{
		std::cout << RED "close read pipe pipe read fd : " << fd << RESET << std::endl;
		close((*moni.clients[fd]).pipe_read[0]);
		moni.all_all_pollfd.erase(moni.all_all_pollfd.begin() + i);
		moni.clients[fd]->responce_cgi = true;
		return true;
	}
	return false;
}

bool Checker::time_out(monitoring &moni, int fd, int i)
{
	int tmp_fd = fd;
    std::map<int, std::string> mapCodeHtml;
    std::map<int, std::string> mapCodeLocation;
   // std::cerr << "tmp: " << tmp_fd << " fd: " << fd << std::endl;
	time_t nowTime = std::time(NULL);
    if (moni.clients.find(fd) != moni.clients.end()) {
        mapCodeHtml = moni.servors[moni.clients[fd]->getFDFather()]->getErrorPage();
        mapCodeLocation = moni.servors[moni.clients[fd]->getFDFather()]->location[moni.clients[fd]->getGoodLocation()].getErrorPage();
    }
	std::map<int, client*>::iterator it = moni.clients.find(fd);
    for (std::map<int, std::string>::iterator ite = mapCodeLocation.begin(); ite != mapCodeLocation.end(); ite++) {
        mapCodeHtml[ite->first] = ite->second;
    }
	if (fd > 0)
	{
		if (it == moni.clients.end())
		{
			tmp_fd = moni.where_are_fd_pipe(fd);
			if (tmp_fd == -1)
				return true;
			if (Checker::if_close_pipe(moni, tmp_fd, i))
				return true;
		}
		if (nowTime - moni.clients[tmp_fd]->getStartTime() >= TIMEOUT)
		{
			if (moni.all_all_pollfd[i].revents & POLLOUT)
			{
				moni.clients[tmp_fd]->setTimeoutBool( true );
				std::string str = "408";

				if (moni.clients[tmp_fd]->getStatusCgi())
					str = "504";
				std::string response = timeout(*moni.clients[tmp_fd], moni, str, mapCodeHtml);
				ssize_t bytes_sent = send(tmp_fd, response.c_str(), response.size(), 0);
				if (bytes_sent == 0)
				{
					std::cerr << RED "Sending Error socket data close" << std::endl;
					delete_client(moni, tmp_fd, i);
					return true;
				}
				else if (bytes_sent == -1)
				{
					std::cerr << RED "Send Error CONNECTION" << std::endl;
					delete_client(moni, tmp_fd, i);
					return true;
				}
				std::cout << "conection close (timeout) " << tmp_fd << ":" << fd << std::endl;
				delete_client(moni, tmp_fd, i);
				return true;
			}
		}
	}
	return false;
}

bool Checker::compar(const int fd, const std::vector<pollfd> &poll_servor)
{

	for (int i = 0; i < (int)poll_servor.size(); ++i)
	{
		if (fd == poll_servor[i].fd)
			return true;
	}

	return false;
}
