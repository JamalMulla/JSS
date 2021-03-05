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

    SCAMP5 s;

    Server ui_server;
    ui_server.start();
    while (1) {
        s.get_image(s.A, s.D);
        ui_server.send_mat(s.A.value());
    }

}

#pragma clang diagnostic pop
