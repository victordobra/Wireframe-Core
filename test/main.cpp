#include <iostream>
#include "Core.hpp"

int main() {
    mge::set<mge::string> set;

    set.insert("bals");
    set.insert("amogus");
    set.insert("sus");
    set.insert("imposter");

    for(const auto& str : set)
        std::cout << str.c_str() << ' ';
    std::cout << '\n';

    set.erase("bals");

    for(const auto& str : set)
        std::cout << str.c_str() << ' ';

    return 0;
}