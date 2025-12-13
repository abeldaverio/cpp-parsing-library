#pragma once

#include "Parser.hpp"
#include <map>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

struct JsonValue;

using JsonArray = std::vector<JsonValue>;
using JsonObject = std::unordered_map<std::string, JsonValue>;

using JsonVariant =
    std::variant<int, double, std::string, bool, JsonObject, JsonArray>;

struct JsonValue {
  JsonValue(JsonVariant const v) : value(v) {}
  JsonValue(int v) : value(v) {}
  JsonValue(double v) : value(v) {}
  JsonValue(const char *v) : value(std::string(v)) {}
  JsonValue(std::string v) : value(std::move(v)) {}
  JsonValue(bool v) : value(v) {}

  JsonVariant value;
};

Parser<JsonValue> parseJsonValue();
Parser<JsonArray> parseJsonArray();
Parser<std::pair<std::string, JsonValue>> parseJsonLine();
Parser<JsonObject> parseJsonObject();
Parser<JsonObject> parseJsonRoot();
