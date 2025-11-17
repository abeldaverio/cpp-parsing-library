#include "BaseTypes.hpp"
#include "Parser.hpp"

Parser<std::string> parseUntil(char c) {
    return parserToString(parseNotChar(c).many() << parseChar(c));
}

Parser<std::string> parseBetween(char c) {
    return parseChar(c) >> parseUntil(c);
}
