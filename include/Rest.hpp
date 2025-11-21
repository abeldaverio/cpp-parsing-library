#pragma once

#include <string>

struct Rest {
    Rest(std::string const &);
    Rest update();
    std::size_t lines;
    std::size_t columns;
    std::string rest;
};
