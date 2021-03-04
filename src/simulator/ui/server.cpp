//
// Created by jm1417 on 04/03/2021.
//

#include <uWebSockets/App.h>
#include <filesystem>
#include <iostream>
#include "simulator/ui/server.h"
#include "simulator/ui/async_file_streamer.h"
#include <thread>

using namespace server;


std::string file_path(const char* path) {
    std::string path_s = path;
    return path_s.substr(0, path_s.find_last_of("\\/"));
}

void server_run() {
    std::cout << "Serving from " << file_path(__FILE__) << std::endl;
    AsyncFileStreamer asyncFileStreamer(file_path(__FILE__));

    uWS::App().get("/*", [&asyncFileStreamer](auto *res, auto *req) {

        /* You can efficiently stream huge files too */
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

void server::start() {
    std::thread server_t (server_run);
    server_t.detach();
}
