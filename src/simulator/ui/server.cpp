//
// Created by jm1417 on 04/03/2021.
//

#include <uWebSockets/App.h>
#include <filesystem>
#include <iostream>
#include "simulator/ui/server.h"
#include "simulator/ui/async_file_streamer.h"

void server::start() {

    std::ifstream myfile;
    myfile.open("index.html");
    if (!myfile.is_open()) {
        std::cout << "Couldn't open file" << std::endl;
    }

    AsyncFileStreamer asyncFileStreamer(".");

    uWS::App().get("/*", [&asyncFileStreamer](auto *res, auto *req) {

        /* You can efficiently stream huge files too */
//        res->writeHeader("Content-Type", "text/html; charset=utf-8")->end("Hello HTTP!");
        serveFile(res, req);
        asyncFileStreamer.streamFile(res, req->getUrl());
    }).ws<UserData>("/*", {

            /* Just a few of the available handlers */
            .open = [](auto *ws) {
                /* MQTT syntax */
                ws->subscribe("sensors/+/house");
            },
            .message = [](auto *ws, std::string_view message, uWS::OpCode opCode) {
                ws->send(message, opCode);
            }

    }).listen(3000, [](auto *listenSocket) {

        if (listenSocket) {
            std::cout << "Listening on port " << 3000 << std::endl;
        }

    }).run();

    std::cout << "Could not start server" << std::endl;
}
