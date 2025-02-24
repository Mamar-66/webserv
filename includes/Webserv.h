#pragma once

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <map>
#include <stack>
#include <fstream>
#include <unistd.h>

#include "Utils.hpp"
#include "Cookie.hpp"

class RequestIn;

std::string makeTheSample(std::string code, std::string str, std::string sample);
std::vector<std::string> StrToVector(std::string str);
std::string getHttpDate();
std::string intToString(int value);
bool isDirectory(const std::string& path);
bool isFile(const std::string& path);
std::map<int, std::string> initCodeMap();
int compNb(std::string s);
int AnalyseStacks(std::stack<char> elems, std::stack<MyTriple<char, char, char> > coma);
char removeSpaces(std::string body, int cas, int i);
int analyseSub(std::string body, int i);
int parseJson(std::string& body);
int parseCatFile(std::string f); //, ParseConfig& conf);
std::string checkRedir(std::string& ch);
std::string concatenateVectors(std::vector<std::string> vec);
std::string loadPage(std::string& catFile);
bool isTypeOk(MyVector<std::string> typesServer, std::string& typeAccepted);
std::vector<std::string> makeAutoIndex( RequestIn& req );
std::string checkCGI(std::string& str);
std::vector<std::string> split(const std::string& str, char delimiter);
MyStack<std::string> create1Stack(const std::string& ch);
void addElemToStr(std::string& str, MyStack<std::string>& stack);
std::string postGetBoundary(std::string& str);
MyTriple<std::string, std::string, bool>  makeFilePost(RequestIn& input);
void createFile(MyTriple<std::string, std::string, bool>& triple);
bool findCookie(MyVector<Cookie> list, std::string id);
std::string generateSessionId();
