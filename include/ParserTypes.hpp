#pragma once

#include "Rest.hpp"
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
    Error() = default;
    Error(std::string const &m, Rest r, bool f = false): message(m), columns(r.columns), lines(r.lines), fatal(f) {}
    std::string message;
    std::size_t columns;
    std::size_t lines;
    bool fatal;
};

template<typename T>
using Result = std::variant<Success<T>, Error>;

enum ResultType {
    SUCCESS = 0,
    ERROR = 1,
};

template<typename ...T>
using MultipleResult = std::variant<MultipleSuccess<T...>, Error>;

template <typename, typename>
struct is_same_template : std::false_type {};

template <template<typename...> class C, typename... Args1, typename... Args2>
struct is_same_template<C<Args1...>, C<Args2...>> : std::true_type {};

template<class A, class B>
concept same_type = is_same_template<A, B>::value;
