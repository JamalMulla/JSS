//
// Created by Jamal on 20/02/2021.
//

#include <iostream>
#include "scamp5.h"
#include <uWebSockets/App.h>
#include <simulator/ui/server.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds


#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#pragma clang diagnostic ignored "-Wmissing-noreturn"

int main() {

    Server ui_server;
    ui_server.start();
    while (1) {
        ui_server.send_string("Some test string");
        std::cout << "Sent string" << std::endl;
        std::this_thread::sleep_for (std::chrono::seconds(5));
    }

}

#pragma clang diagnostic pop
