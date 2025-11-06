#pragma once

#include "Parser.hpp"
#include "ParserTypes.hpp"
#include "Rest.hpp"
#include <any>

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
Parser<T> parsingError(std::string const &err) {
    return Parser<T>([err](Rest r) {
       return Error(err, r);
    });
}
