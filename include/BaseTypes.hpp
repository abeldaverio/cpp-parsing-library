#pragma once
#include "Parser.hpp"

Parser<char> parseChar(char c);
Parser<char> parseCharFromList(std::string const &list);
Parser<std::string> parserToString(Parser<std::vector<char>> p);
Parser<int> parseInt();
Parser<std::string> parseStringDidgit();
Parser<double> parseDouble();
