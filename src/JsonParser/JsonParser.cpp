#include "Json/JsonParser.hpp"
#include "BaseTypes.hpp"
#include "Parser.hpp"
#include "ParserUtils.hpp"
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

Parser<std::string> parseJsonComment() {
    return
        (parseString("//") >> parseUntil('\n')) ||
        (parseBetweenString("/*", "*/"));
}

Parser<std::vector<std::string>> jsonSkip() {
    return (parseJsonComment() || parserToString((parseCharFromList("\t\n\r ")).some())).many();
}

template<typename T>
JsonVariant convertToVarient(T v) {
  return v;
};

template<typename T>
Parser<JsonVariant> variant(Parser<T> p) {
    return apply(convertToVarient<T>, p);
}

Parser<bool> parseJsonBool() {
    return parseString("true") >> pure(true) || parseString("false") >> pure(false);
}

Parser<JsonArray> parseJsonArray() {
    return (
        parseChar('[') >>
            (parsingContext<JsonArray>("Parsing json array", true) >=
                parseMultipleWithSeparator(jsonSkip() >> parseJsonValue() << jsonSkip(), ','))
        << parseChar(']')
    );
}

Parser<JsonValue> parseJsonValue() {
    return apply([](JsonVariant v) -> JsonValue {return {v};},
        variant(parseDouble()) ||
        variant(parseInt()) ||
        variant(parseBetween('"')) ||
        variant(parseJsonBool()) ||
        variant(lazy(parseJsonArray)) ||
        variant(lazy(parseJsonObject)) ||
        parsingError<JsonVariant>("unknow value")
    );
}

Parser<std::pair<std::string, JsonValue>> parseJsonLine() {
    return apply([](std::string k, JsonValue v){return std::make_pair(k, v);},
        jsonSkip() >> (parseBetween('"') || parsingError<std::string>("Error parsing json key")) << jsonSkip() << parseChar(':'),
        jsonSkip() >> (parsingContext<JsonValue>("Parsing json value", true) >= parseJsonValue()) << jsonSkip()
    );
}

Parser<JsonObject> parseJsonRoot() {
    return apply(
        [](std::vector<std::pair<std::string, JsonValue>> vect) {
            return std::unordered_map<std::string, JsonValue>(vect.begin(), vect.end());
        },
        parseMultipleWithSeparator(parseJsonLine(), ',')
    );
}

Parser<JsonObject> parseJsonObject() {
    return (parsingContext<JsonObject>("Parsing json object") >= ( jsonSkip() >>
        parseChar('{') >>
             (parsingContext<JsonObject>("Parsing json object", true) >= parseJsonRoot()))
        << (parseChar('}') || parsingError<char>("Missing char end of object \"}\"", true)) << jsonSkip()
    );
}
