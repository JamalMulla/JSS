//
// Created by Jamal on 20/02/2021.
//

#include <iostream>
#include "scamp5.h"
#include <uWebSockets/App.h>
#include <simulator/ui/server.h>


#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#pragma clang diagnostic ignored "-Wmissing-noreturn"

int main() {

    server::start();
    std::cout << "Will also run other code" << std::endl;
    while (1) {

    }

}

#pragma clang diagnostic pop
