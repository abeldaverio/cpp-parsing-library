#include "BaseTypes.hpp"
#include "Parser.hpp"
#include "ParserUtils.hpp"
#include <cctype>
#include <cstdint>
#include <string>

Parser<char> parseDigit() { return parseCharFromList("0123456789"); }

Parser<char> ParseCharAndLower(char c) {
  return parseChar(c) || parseChar(std::tolower(c));
}

Parser<std::string> parseStringDigitBase(std::string const &base) {
  Parser<char> p = parsingError<char>("error parsing digit, no digit");

  for (char c : base) {
    p = ParseCharAndLower(c) || p;
  }
  return parserToString(p.some());
}

Parser<std::uint64_t> parseUnsignedIntBase(std::string const &base) {
  return apply(
      [base](std::string str) { return std::stoul(str, 0, base.size()); },
      parseStringDigitBase(base));
}

Parser<std::uint64_t> parseUnsignedHexa() {
  return parseChar('0') >> ParseCharAndLower('X') >>
         parseUnsignedIntBase("0123456789ABCDEF");
}

Parser<std::uint64_t> parseUnsignedOctal() {
  return parseChar('0') >> parseUnsignedIntBase("01234567");
}

Parser<std::uint64_t> parseUnsignedBinary() {
  return parseChar('0') >> ParseCharAndLower('B') >> parseUnsignedIntBase("01");
}

Parser<std::uint64_t> parseLongUnsignedInt() {
  return parseUnsignedHexa() || parseUnsignedBinary() || parseUnsignedOctal() ||
         parseUnsignedIntBase("0123456789");
}

Parser<std::uint32_t> parseUnsignedInt() {
  return PCAST(parseLongUnsignedInt(), std::uint32_t);
}

Parser<std::int64_t> parseLongInt() {
  return (parseChar('-') >>
          apply([](std::uint64_t i) { return -(std::int64_t)i; },
                parseUnsignedInt())) ||
         PCAST(parseLongUnsignedInt(), std::int64_t);
}

Parser<int> parseInt() { return PCAST(parseLongInt(), int); }

Parser<double> parseUnsignedDouble() {
  return apply(
             [](std::string str) { return std::stod(str); },
             apply([](std::string i, std::string dec) { return i + "." + dec; },
                   parseStringDigitBase("0123456789"),
                   parseChar('.') >> (parseStringDigitBase("0123456789") ||
                                      pure<std::string>("0")))) ||
         parsingError<double>("Error parsing double");
}

Parser<double> parseDouble() {
  return (parseChar('-') >>
          apply([](double i) { return -i; }, parseUnsignedDouble())) ||
         parseUnsignedDouble();
}
