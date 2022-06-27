#include <iostream>
#include "Core.hpp"

int main() {
    mge::FileInput input("Cube.obj");

    while(!input.IsAtTheEnd()) {
        mge::string line = "";
        input.ReadLine(line, 1000001);
        const char* cStr = line.c_str();
        std::cout << cStr;
    }

    input.Close();

    return 0;
}