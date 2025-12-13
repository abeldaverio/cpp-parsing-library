#pragma once

#include <algorithm>
#include <bit>
#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

#include <stdint.h>
#include <sys/types.h>

#include "BaseTypes.hpp"
#include "Json/JsonParser.hpp"
#include "Parser.hpp"
#include "ParserUtils.hpp"

template<typename Type>
Parser<Type> parseByte(std::endian endian = std::endian::big)
{
  return apply(
      [endian](std::vector<char> v)
      {
        if (std::endian::native != endian) {
          std::reverse(v.begin(), v.end());
        }
        return *(Type*)v.data();
      },
      parseAnyChar() * sizeof(Type));
}

template<typename Type>
Parser<std::vector<Type>> parseByteArray(Parser<Type> p)
{
  return parseByte<u_int32_t>() >>=
      std::function<Parser<std::vector<Type>>(u_int32_t)>([p](u_int32_t nb)
                                                          { return p * nb; });
}

template<typename Key, typename Value>
Parser<std::pair<Key, Value>> parseBytePair(Parser<Key> k, Parser<Value> v)
{
  return apply([](Key const& key, Value const& value)
               { return std::make_pair(key, value); },
               k,
               v);
}

template<typename Key, typename Value>
Parser<std::unordered_map<Key, Value>> parseByteMap(Parser<Key> k,
                                                    Parser<Value> v)
{
  return apply([](std::vector<std::pair<Key, Value>> v)
               { return std::unordered_map<Key, Value>(v.begin(), v.end()); },
               parseByteArray(parseBytePair(k, v)));
}

template<typename Type>
Parser<std::optional<Type>> parseByteOptional(Parser<Type> p)
{
  return parseByte<bool>() >>= std::function<Parser<std::optional<Type>>(bool)>(
             [p](bool exist)
             {
               if (exist) {
                 return apply([](Type const& t) { return std::optional(t); },
                              p);
               }
               return pure<std::optional<Type>>(std::nullopt);
             });
}

Parser<std::string> parseByteString();
Parser<JsonValue> parseByteJsonValue();
Parser<JsonObject> parseByteJsonObject();
