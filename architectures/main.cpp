//
// Created by Jamal on 20/02/2021.
//

#include <iostream>
#include "scamp5.h"
#include <uWebSockets/App.h>
#include <simulator/ui/server.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <simulator/util/utility.h>


#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#pragma clang diagnostic ignored "-Wmissing-noreturn"

int main() {

    SCAMP5 s;

    Server ui_server;
    ui_server.start();
    while (1) {
        s.get_image(s.A, s.D);
        s.neg(s.NEWS, s.A);
        ui_server.send_mat("A", utility::remap_image(s.A).value());
        ui_server.send_mat("D", utility::remap_image(s.D).value());
        ui_server.send_mat("NEWS", utility::remap_image(s.NEWS).value());

        cv::waitKey(1);
    }

}

#pragma clang diagnostic pop
