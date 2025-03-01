#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

class Conversion {
	private:
		Conversion();
		~Conversion();
		Conversion(Conversion& cpy);
		Conversion& operator=(Conversion& cpy);

	public:
		static int	stringToInt(std::string& str);
		static std::string	intToString(int value);
		static std::string vectorToString(std::vector<std::string> vec);
		static std::vector<std::string> StrToVector(std::string str);
};
