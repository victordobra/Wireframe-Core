#include <iostream>
#include "Core.hpp"

int main() {
    mge::vector<int> vec{1, 2, 3};

    for(auto number : vec)
        std::cout << number << ' ';

    return 0;
}