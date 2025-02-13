
#include "../../../includes/Parser.hpp"

int AnalyseStacks(std::stack<char> elems, std::stack<MyTriple<char, char, char> > comas)
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


char removeSpaces(std::string body, int cas, int i) {
    switch (cas) {
        case BEFORE:
            i--;
            while (body[i] == ' ')
                i--;
            return body[i];
        case AFTER:
            i++;
            while (body[i] == ' ')
                i++;
            return body[i];
        default:
            return ('?');
    }
}

int analyseSub(std::string body, int i) {
    
    if (removeSpaces(body, BEFORE, i) != '"')
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

int parseCatFile(std::string f) { //, ParseConfig& conf) {
    std::istringstream iss(f);
    std::istringstream is2("./"); // Change to conf.getRoot()

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

int parseJson(std::string& body) {
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
            stackComa.push(MyTriple<char, char, char>(removeSpaces(body, BEFORE, i), body[i], removeSpaces(body, AFTER, i)));
        if (body[i] == ':' && !(analyseSub(body, i)))
            return 400;
    }
    stackElem.push(body[body.size() - 1]);

    return AnalyseStacks(stackElem, stackComa);
}


bool isTypeOk(MyVector<std::string> typesServer, std::string& typeAccepted) {
    MyVector<std::string> types;
    std::istringstream iss(typeAccepted);
    std::string type;

    while (std::getline(iss, type, ',')) {
        type.erase(std::remove(type.begin(), type.end(), ' '));
        types.push_back(type);
    }

    for (std::vector<std::string>::iterator it = types.begin(); it != types.end(); it++) {
        if (typesServer == *it) 
            return true;
    }
    return false;
}
