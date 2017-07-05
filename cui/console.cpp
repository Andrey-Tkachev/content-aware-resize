//
// Created by nikita on 05.07.17.
//
#include <iostream>
#include "config.h"

int main() {
    Config &c = Singleton<Config>::Instance();
    std::cout << c["directory"];
    for (int i = 0; i!= 10; ++i) {
        std::cout << i << "lal";
    }
    

    return 0;
}