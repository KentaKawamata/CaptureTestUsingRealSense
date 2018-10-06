//
// Created by kawa on 10/6/18.
//
#include <iostream>
#include <sstream>
#include "capture.h"

int main(int argc, char* argv[]) {

    try {

        Capture capture;
        capture.run();

    } catch (std::exception &ex) {
        std::cout << ex.what() << std::endl;
    }
    return 0;
}