#include "Rest.hpp"
#include "ParserTypes.hpp"
#include <cstddef>
#include <format>
#include <iostream>
#include <string>
#include <vector>

Rest::Rest(std::string const &str): lines(1), columns(1), rest(str) {}
Rest::Rest(std::vector<char> const &v): lines(1), columns(1), rest(v.begin(), v.end()) {}
Rest::Rest(std::vector<unsigned char> const &v): lines(1), columns(1), rest(v.begin(), v.end()) {}

Rest Rest::update() {
    if (this->rest.empty()) {
        return *this;
    }
    if (this->rest.starts_with('\n')) {
        this->columns = 1;
        this->lines++;
    } else {
        this->columns++;
    }
    this->rest = this->rest.substr(1, this->rest.length());
    return *this;
}

std::ostream &operator<<(std::ostream &os, Error const &err) {
    return os << std::format("At line {} and col {}:\n\t{}: {}", err.rest.lines, err.rest.columns, err.context, err.message);
}

static std::string getLineFromBase(std::size_t line, std::string const base) {
    long unsigned int start = 1;
    long unsigned int end = 1;

    while (line > 0 && start < base.size()) {
        if (base[start] == '\n') {
            line--;
        }
        start++;
    }

    for (end = start; end < base.size() && base[end] != '\n'; end++);
    return base.substr(start, end - start);
}

void printError(Error const &error, std::string const &base, std::string const &filename) {
    std::cout << "\033[0;1;4m" << filename << ":" << error.rest.lines << ":" << error.rest.columns << "\033[0m: "
        << error.context << ": " << "\033[31;1m" << error.message << "\033[0m" << std::endl << std::endl
        << error.rest.lines << " |" << getLineFromBase(error.rest.lines, base) << std::endl
        << "  |" << std::string(error.rest.columns, ' ') << "\033[34m" << '^' << "\033[0m" << std::endl;
}
