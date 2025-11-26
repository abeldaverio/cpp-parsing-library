#pragma once
#include "Parser.hpp"

Parser<char> parseChar(char c);
Parser<char> parseAnyChar();
Parser<char> parseNotChar(char c);
Parser<char> parseCharFromList(std::string const &list);
Parser<std::string> parserToString(Parser<std::vector<char>> p);
Parser<std::string> parseString(std::string const &str);
Parser<char> parseDigit();
Parser<char> ParseCharAndLower(char c);
Parser<std::string> parseStringDigitBase(std::string const &base);
Parser<long unsigned int> parseUnsignedIntBase(std::string const &base);
Parser<long unsigned int> parseUnsignedHexa();
Parser<long unsigned int> parseUnsignedOctal();
Parser<long unsigned int> parseUnsignedBinary();
Parser<long unsigned int> parseLongUnsignedInt();
Parser<unsigned int> parseUnsignedInt();
Parser<long int> parseLongInt();
Parser<int> parseInt();
Parser<double> parseDouble();
Parser<char> parseCharNotString(std::string const str);
