#pragma once

#include <string>
#include <vector>

struct Rest {
    Rest(std::string const &);
    Rest(std::vector<char> const &);
    Rest(std::vector<unsigned char> const &);
    Rest update();
    std::size_t lines;
    std::size_t columns;
    std::string rest;
};
