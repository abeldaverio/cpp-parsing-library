#pragma once

#include "BaseTypes.hpp"
#include "Parser.hpp"
#include "ParserUtils.hpp"
#include <algorithm>
#include <bit>
#include <functional>
#include <stdint.h>
#include <sys/types.h>
#include <vector>

template <typename Type>
Parser<Type> parseByte(std::endian endian = std::endian::big) {
  return apply(
      [endian](std::vector<char> v) {
        if (std::endian::native != endian) {
          std::reverse(v.begin(), v.end());
        }
        return *(Type *)v.data();
      },
      parseAnyChar() * sizeof(Type));
}

template <typename Type>
Parser<std::vector<Type>> parseByteArray(Parser<Type> p) {
  return parseByte<u_int32_t>() >>=
         std::function<Parser<std::vector<Type>>(u_int32_t)>(
             [p](u_int32_t nb) { return p * nb; });
}

Parser<std::string> parseByteString();
