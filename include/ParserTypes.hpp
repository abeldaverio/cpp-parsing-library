#pragma once

#include "Rest.hpp"
#include <ostream>
#include <string>
#include <variant>


template<typename T>
struct Success {
    T value;
    Rest rest;
};

template<typename ...T>
struct MultipleSuccess {
    std::tuple<T...> values;
    Rest rest;
};

struct Error {
    Error(std::string const &m, Rest r, bool f = false, std::string const &context = "")
        : message(m), context(context), rest(r), fatal(f) {}
    std::string message;
    std::string context;
    Rest rest;
    bool fatal;
};

void printError(Error const &error, std::string const &base, std::string const &filename = "");
std::ostream &operator<<(std::ostream &os, Error const &err);

template<typename T>
using Result = std::variant<Success<T>, Error>;

enum ResultType {
    SUCCESS = 0,
    ERR = 1,
};

template<typename ...T>
using MultipleResult = std::variant<MultipleSuccess<T...>, Error>;

template <typename, typename>
struct is_same_template : std::false_type {};

template <template<typename...> class C, typename... Args1, typename... Args2>
struct is_same_template<C<Args1...>, C<Args2...>> : std::true_type {};

template<class A, class B>
concept same_type = is_same_template<A, B>::value;
