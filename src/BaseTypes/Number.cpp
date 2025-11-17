#include "Parser.hpp"
#include "BaseTypes.hpp"
#include "ParserUtils.hpp"
#include <cctype>
#include <string>

Parser<char> parseDigit() {
    return parseCharFromList("0123456789");
}

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

Parser<long unsigned int> parseUnsignedIntBase(std::string const &base) {
    return apply([base](std::string str) {return std::stoul(str, 0, base.size());}, parseStringDigitBase(base));
}

Parser<long unsigned int> parseUnsignedHexa() {
    return parseChar('0') >> ParseCharAndLower('X') >>
        parseUnsignedIntBase("0123456789ABCDEF");
}

Parser<long unsigned int> parseUnsignedOctal() {
    return parseChar('0') >> parseUnsignedIntBase("01234567");
}

Parser<long unsigned int> parseUnsignedBinary() {
    return parseChar('0') >> ParseCharAndLower('B') >> parseUnsignedIntBase("01");
}

Parser<long unsigned int> parseLongUnsignedInt() {
    return parseUnsignedHexa() || parseUnsignedBinary() || parseUnsignedOctal() || parseUnsignedIntBase("0123456789");
}

Parser<unsigned int> parseUnsignedInt() {
    return PCAST(parseLongUnsignedInt(), unsigned int);
}

Parser<long int> parseLongInt() {
    return (parseChar('-') >> apply([](long unsigned int i) {return -(long int)i;}, parseUnsignedInt()))
        || PCAST(parseLongUnsignedInt(), long int);
}

Parser<int> parseInt() {
    return PCAST(parseLongInt(), int);
}

Parser<double> parseDouble() {
    return apply([](std::string str) {return std::stod(str);},
        apply([](std::string i, std::string dec) { return i + "." + dec;},
            parseStringDigitBase("0123456789"),
            parseChar('.') >> (parseStringDigitBase("0123456789") || pure<std::string>("0")))
    ) || parsingError<double>("Error parsing double");
}
