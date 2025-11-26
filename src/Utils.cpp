#include "BaseTypes.hpp"
#include "Parser.hpp"
#include "ParserUtils.hpp"
#include <string>

Parser<std::string> parseUntil(char c) {
    return parserToString(parseNotChar(c).many());
}

Parser<std::string> parseUntilString(std::string const &str) {
    return parserToString(parseCharNotString(str).many());
}

Parser<std::string> parseBetween(char c, char c2) {
    c2 = c2 != -1 ? c2 : c;
    return parseChar(c) >> parseUntil(c2) << parseChar(c2);
}

Parser<std::string> parseBetweenString(const std::string &str, std::optional<std::string> str2) {
    std::string end = str2.has_value() ? str2.value() : str;
    return parseString(str) >> parseUntilString(end) << parseString(end);
}
