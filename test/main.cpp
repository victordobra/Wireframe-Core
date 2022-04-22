#include <iostream>
#include "IO.hpp"
#include "map.hpp"
#include "unordered_map.hpp"

struct sussy {
    mge::uint32_t a, b;
};

namespace mge {
    template<>
    string ToString(sussy sus) {
        string str = ToString(sus.a) + " " + ToString(sus.b);
        return str;
    }
}

int main(int, char**) {
    mge::unordered_map<int, mge::string> map;
    mge::string str1 = "sus", str2 = "sussy", str3 = "amogus";

    map[2] = str1;
    map[1] = str2;
    map[5] = str3;

    std::cout << map[1].c_str() << ' ' << map[2].c_str() << ' ' << map[5].c_str();

    return 0;
}