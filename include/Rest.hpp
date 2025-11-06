#pragma once

#include <string>

struct Rest {
    Rest(std::string const &);
    Rest update();
    std::size_t columns;
    std::size_t lines;
    std::string rest;
};
