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
Parser<std::uint64_t> parseUnsignedIntBase(std::string const &base);
Parser<std::uint64_t> parseUnsignedHexa();
Parser<std::uint64_t> parseUnsignedOctal();
Parser<std::uint64_t> parseUnsignedBinary();
Parser<std::uint64_t> parseLongUnsignedInt();
Parser<std::uint32_t> parseUnsignedInt();
Parser<std::int64_t> parseLongInt();
Parser<int> parseInt();
Parser<double> parseUnsignedDouble();
Parser<double> parseDouble();
Parser<char> parseCharNotString(std::string const str);
