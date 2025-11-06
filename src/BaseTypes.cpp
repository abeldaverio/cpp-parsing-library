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

Parser<char> parseDigit() {
    return parseCharFromList("0123456789");
}

Parser<std::string> parseStringDidgit() {
    return parserToString(parseDigit().some()) ||
        parsingError<std::string>("error parsing digit, no digit");
}

Parser<int> parseInt() {
    return apply([](std::string str) {return std::stoi(str);}, parseStringDidgit());
}

Parser<double> parseDouble() {
    return apply([](std::string str) {return std::stod(str);},
        apply([](std::string i, std::string dec) { return i + "." + dec;},
            parseStringDidgit(), parseChar('.') >> (parseStringDidgit() || pure<std::string>("0")))
    ) || parsingError<double>("Error parsing double");
}
