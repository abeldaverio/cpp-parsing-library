#include <string>
#include "Parser.hpp"
#include "ParserTypes.hpp"
#include "ParserUtils.hpp"

Parser<char> parseChar(char c) {
    return Parser<char>([c](Rest rest) -> Result<char> {
       if (rest.rest.empty()) {
           return Error(
               std::format("Error parsing char \"{}\", string empty", c),
               rest
           );
       }
       if (rest.rest.starts_with(c)) {
           return Success<char>{*rest.rest.begin(), rest.update()};
       } else {
           return Error(
               std::format("Error parsing char \"{}\", char is \"{}\"", c, *rest.rest.begin()),
               rest
           );
       }
    });
}

Parser<char> parseNotChar(char c) {
    return Parser<char>([c](Rest rest) -> Result<char> {
       if (rest.rest.empty()) {
           return Error(
               std::format("Error parsing char \"{}\", string empty", c),
               rest
           );
       }
       if (!rest.rest.starts_with(c)) {
           return Success<char>{*rest.rest.begin(), rest.update()};
       } else {
           return Error(
               std::format("Error parsing not char \"{}\", char is \"{}\"", c, *rest.rest.begin()),
               rest
           );
       }
    });
}

Parser<char> parseAnyChar() {
    return Parser<char>([](Rest rest) -> Result<char> {
       if (rest.rest.empty()) {
           return Error(
               std::format("Error parsing any char, string empty"),
               rest
           );
       }
       return Success<char>{*rest.rest.begin(), rest.update()};
    });
}

Parser<char> parseCharFromList(std::string const &list) {
    return Parser<char>([list](Rest rest) -> Result<char> {
       if (rest.rest.empty()) {
           return Error(
               std::format("Error parsing char from list \"{}\", string empty", list),
               rest
           );
       }
       if (list.contains(*rest.rest.begin())) {
           return Success<char>{*rest.rest.begin(), rest.update()};
       } else {
           return Error(
               std::format("Error parsing char \"{}\", from list \"{}\"", *rest.rest.begin(), list),
               rest
           );
       }
    });
}

Parser<std::string> parserToString(Parser<std::vector<char>> p) {
    return apply([](std::vector<char> v) {return std::string(v.data(), v.size());}, p);
}

Parser<std::string> parseString(std::string const &str) {
    return Parser<std::string>([str](Rest rest) -> Result<std::string> {
        for (char c : str) {
            if (rest.rest.empty()) {
                return Error(std::format("error parsing string \"{}\", rest empty", str), rest);
            }
            if (c != rest.rest[0]) {
                return Error(
                    std::format("error parsing string \"{}\", {} is not equal to {}", str, c, rest.rest[0]),
                    rest
                );
            }
            rest.update();
        }
        return Success<std::string>(str, rest);
    });
}

Parser<char> parseCharNotString(std::string const str) {
    return Parser<char>([str](Rest rest) -> Result<char> {
        if (rest.rest.starts_with(str)) {
            return Error(std::format("error parsing char \"{}\", string \"{}\" found", rest.rest[0], str), rest);
        }
        return parseAnyChar()(rest);
    });
}
