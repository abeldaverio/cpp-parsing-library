#pragma once

#include "Parser.hpp"
#include <map>
#include <string>
#include <variant>
#include <vector>


struct JsonValue;

using JsonArray  = std::vector<JsonValue>;
using JsonObject = std::map<std::string, JsonValue>;

using JsonVariant = std::variant<
    int,
    double,
    std::string,
    bool,
    JsonObject,
    JsonArray
>;

struct JsonValue {
    JsonVariant value;
};

Parser<JsonValue> parseJsonValue();
Parser<std::pair<std::string, JsonValue>> parseJsonLine();
Parser<JsonObject> parseJsonObject();
Parser<JsonObject> parseJsonRoot();
