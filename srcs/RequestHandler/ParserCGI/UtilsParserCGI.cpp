#include "../../../includes/Parser.hpp"

// std::string checkCGI(std::string& str) {
//     MyStack<std::string> stackPath = create1Stack(str);
//     std::string stringCGI = "";
//     addElemToStr(stringCGI, stackPath);
//     while ( ( Checker::isFile(stringCGI) == false || access(stringCGI.c_str(), X_OK) != 0 ) && stackPath.empty() == false) {
//         addElemToStr(stringCGI, stackPath);
//         std::cout << stackPath.empty() << std::endl;
//     }
//     if (Checker::isFile(stringCGI) == true && access(stringCGI.c_str(), X_OK) == 0)
//         return stringCGI;
//     return "";
// }
