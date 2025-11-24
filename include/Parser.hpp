#pragma once

#include "ParserTypes.hpp"
#include "Rest.hpp"
#include <functional>
#include <ostream>

template<typename T>
class Parser {
    public:

    using value_type = T;

    Parser(std::function<Result<T> (Rest)> function): function_(function) {}

    Result<T> operator()(Rest rest) const {
        return this->function_(rest);
    }

    Parser<T> operator||(Parser<T> const &other) {
        return Parser<T>(
            [self = *this, other](Rest r) {
            Result<T> first = self(r);

            if (first.index() == SUCCESS || std::get<ERROR>(first).fatal) {
                return first;
            }
            return other(r);
        });
    }

    Parser<T> operator>=(Parser<T> const &other) {
        return Parser<T>(
            [self = *this, other](Rest r) -> Result<T> {
            Result<T> first = self(r);

            if (first.index() == SUCCESS) {
                return first;
            }
            Result<T> second = other(r);

            if (second.index() == SUCCESS) {
                return second;
            }
            Error const &eFirst = std::get<ERROR>(first);
            Error const &eSecond = std::get<ERROR>(second);
            return Error(eSecond.message, eSecond.rest, eFirst.fatal | eSecond.fatal, eFirst.context);
        });
    }

    Parser<T> operator<=(Parser<T> const &other) {
        return Parser<T>(
            [self = *this, other](Rest r) -> Result<T> {
            Result<T> second = other(r);

            if (second.index() == SUCCESS) {
                return second;
            }
            Result<T> first = self(r);

            if (first.index() == SUCCESS) {
                return first;
            }

            Error const &eFirst = std::get<ERROR>(first);
            Error const &eSecond = std::get<ERROR>(second);
            return Error(eFirst.message, eFirst.rest, eFirst.fatal | eSecond.fatal, eSecond.context);
        });
    }

    template<typename O>
    Parser<O> operator>>(Parser<O> const real) {
        return Parser<O>([self = *this, real](Rest r) -> Result<O> {
            Result<T> f = self(r);

            if (f.index() == ERROR) {
                return std::get<ERROR>(f);
            } else {
                return real(std::get<SUCCESS>(f).rest);
            }
        });
    }

    template<typename O>
    Parser<T> operator<<(Parser<O> const other) {
        return Parser<T>([self = *this, other](Rest r) -> Result<T> {
            Result<T> f = self(r);

            if (f.index() == ERROR) {
                return std::get<ERROR>(f);
            }
            Result<O> o = other(std::get<SUCCESS>(f).rest);
            if (o.index() == ERROR) {
                return std::get<ERROR>(o);
            }
            return Success<T>{std::get<SUCCESS>(f).value, std::get<SUCCESS>(o).rest};
        });
    }

    Parser<std::vector<T>> many() const {
        return Parser<std::vector<T>>([self = *this](Rest rest) -> Result<std::vector<T>> {
            std::vector<T> result;
            Result<T> step = self(rest);

            while (step.index() == SUCCESS) {
                result.push_back(std::get<SUCCESS>(step).value);
                rest = std::get<SUCCESS>(step).rest;
                step = self(rest);
            }
            if (std::get<Error>(step).fatal) {
                return std::get<Error>(step);
            }
            return Success<std::vector<T>>{std::move(result), rest};
        });
    }

    template<typename O>
    Parser<O> operator>>=(std::function<Parser<O> (T)> func) {
        return Parser<O>([self = *this, func](Rest rest) -> Result<O> {
            Result<T> fst = self(rest);
            if (fst.index() == ERROR) {
                return std::get<ERROR>(fst);
            } else {
                return func(std::get<SUCCESS>(fst).value)(std::get<SUCCESS>(fst).rest);
            }
        });
    }

    Parser<std::vector<T>> some() const {
        return Parser<std::vector<T>>([self = *this](Rest rest) -> Result<std::vector<T>> {
            Result<std::vector<T>> manyParsed = (self.many())(rest);
            if (std::get<SUCCESS>(manyParsed).value.size() == 0) {
                return Error("Error parsing some, no result", rest);
            } else {
                return manyParsed;
            }
        });
    }

    private:
     std::function<Result<T> (Rest)> function_;

};

template<typename T>
std::ostream &operator<<(std::ostream &os, Result<T> const &obj) {
    try {
        Success<T> const &result = std::get<Success<T>>(obj);
        return (
        os << '('
            << result.value << ", (\"" << result.rest.rest << "\", "
            << result.rest.lines << ", " << result.rest.columns
        << ")"
        );
    } catch (const std::bad_variant_access &) {
        Error err = std::get<ERROR>(obj);
        return os << "at line " << err.rest.lines << " column " << err.rest.columns << ": " << err.message;
    }
}
