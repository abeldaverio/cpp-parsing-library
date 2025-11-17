#pragma once

#include "Parser.hpp"
#include "ParserTypes.hpp"
#include "Rest.hpp"
#include "BaseTypes.hpp"
#include <any>
#include <format>
#include <vector>

template<typename T>
Parser<T> pure(T value) {
    return Parser<T>([value](Rest r) {
        return Success<T>{value, r};
    });
}

template<std::size_t... Is, typename ...Type>
MultipleResult<typename Type::value_type...> for_each_parser(Rest str, std::tuple<Type...>& parsers, std::index_sequence<Is...>) {
    Rest rest = str;
    std::tuple<typename Type::value_type...> values;

    try {
        (([&]() {
            auto result = std::get<Is>(parsers)(rest);

            if (result.index() == ERROR) {
                throw std::get<ERROR>(result);
            } else {
                auto success = std::get<SUCCESS>(result);
                std::get<Is>(values) = success.value;
                rest = success.rest;
            }
        })(), ...);
        return MultipleSuccess<typename Type::value_type...>{values, rest};
    } catch (Error error) {
        return error;
    }
}

template <typename F, same_type<Parser<std::any>>... Parsers>
Parser<std::invoke_result_t<F, typename Parsers::value_type...>> apply(F&& f, Parsers... parsers) {
    using R = std::invoke_result_t<F, typename Parsers::value_type...>;

    return Parser<R>([=](Rest rest) -> Result<R> {
        auto parserList = std::make_tuple(parsers...);
        auto result = for_each_parser(rest, parserList,
            std::make_index_sequence<std::tuple_size_v<decltype(parserList)>>{});

        if (result.index() == ERROR) {
            return std::get<ERROR>(result);
        }
        auto success = std::get<SUCCESS>(result);
        return Success<R>{std::apply(f, success.values), success.rest};
    });
}

template<typename T>
Parser<T> parsingError(std::string const &err, bool fatal = false) {
    return Parser<T>([err, fatal](Rest r) {
       return Error(err, r, fatal);
    });
}

template<typename T>
Parser<T> parsingContext(std::string const &context, bool fatal = false) {
    return Parser<T>([context, fatal](Rest r) {
        Error err("", r, fatal);
        err.context = context;
        return err;
    });
}


#define PCAST(p, type) (apply([](auto v) {return (type)(v);}, p))

template<typename T>
Parser<std::vector<T>> parseMultipleWithSeparator(Parser<T> p, char sep) {
    return apply([](T first, std::vector<T> rest) {
        rest.insert(rest.begin(), first);
        return rest;
    }, p, ((parseChar(sep) ||
            parsingError<char>(std::format("Error parsing multiple elements: missing char \"{}\"", sep)))
            >> p).many()) ||
        pure(std::vector<T>());
}

template<typename T, typename ...Args>
Parser<T> lazy(std::function<Parser<T> (Args...)> f, Args... args) {
    return Parser<T>([=](Rest r) {
        return f(args...)(r);
    });
}

template<typename F>
auto lazy(F f) {
    using ParserType = decltype(f());
    using ValueType = typename ParserType::value_type;

    return Parser<ValueType>(
        [f](Rest r) {
            return f()(r);
        }
    );
}

Parser<std::string> parseUntil(char c);
Parser<std::string> parseBetween(char c);
