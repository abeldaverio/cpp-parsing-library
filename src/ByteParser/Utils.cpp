#include "BaseTypes.hpp"
#include "ByteParser/ByteParser.hpp"
#include "Parser.hpp"
#include <string>

Parser<std::string> parseByteString() {
  return parserToString(parseByteArray(parseAnyChar()));
}
