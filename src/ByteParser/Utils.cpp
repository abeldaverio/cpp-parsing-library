#include "BaseTypes.hpp"
#include "ByteParser/ByteParser.hpp"
#include "Parser.hpp"
#include "ParserUtils.hpp"
#include "Json/JsonParser.hpp"
#include <cstdint>
#include <string>

Parser<std::string> parseByteString() {
  return parserToString(parseByteArray(parseAnyChar()));
}

static const std::array<std::function<Parser<JsonValue>()>, 6> json_value_map =
    {
        []() {
          return apply([](int i) { return JsonValue(i); }, parseByte<int>());
        },
        []() {
          return apply([](double i) { return JsonValue(i); },
                       parseByte<double>());
        },
        []() {
          return apply([](std::string i) { return JsonValue(i); },
                       parseByteString());
        },
        []() {
          return apply([](bool i) { return JsonValue(i); }, parseByte<int>());
        },
        []() {
          return apply([](JsonObject i) { return JsonValue(i); },
                       parseJsonObject());
        },
        []() {
          return apply([](JsonArray i) { return JsonValue(i); },
                       parseByteArray(parseJsonValue()));
        },
};

Parser<JsonValue> parseJsonValue() {
  return parseByte<std::uint8_t>() >>=
         std::function<Parser<JsonValue>(std::uint8_t)>(
             [](std::uint8_t i) { return json_value_map[i](); });
}

Parser<JsonObject> parseByteJsonObject() {
  return parseByteMap(parseByteString(), parseJsonValue());
}
