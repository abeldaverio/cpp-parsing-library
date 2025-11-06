#include "Rest.hpp"
#include <string>

Rest::Rest(std::string const &str): lines(0), columns(0), rest(str) {}


Rest Rest::update() {
    if (this->rest.empty()) {
        return *this;
    }
    if (this->rest.starts_with('\n')) {
        this->columns = 0;
        this->lines++;
    } else {
        this->columns++;
    }
    this->rest = this->rest.substr(1, this->rest.length());
    return *this;
}
