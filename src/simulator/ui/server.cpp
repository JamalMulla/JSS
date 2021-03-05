//
// Created by jm1417 on 04/03/2021.
//

#include <uWebSockets/App.h>
#include <filesystem>
#include <iostream>
#include "simulator/ui/server.h"
#include "simulator/ui/async_file_streamer.h"
#include <thread>


std::string file_path(const char* path) {
    std::string path_s = path;
    return path_s.substr(0, path_s.find_last_of("\\/"));
}

void Server::server_run() {
    std::string serving_dir = file_path(__FILE__) + std::filesystem::path::preferred_separator + "src";

    AsyncFileStreamer asyncFileStreamer(serving_dir);
    //todo refactor into class

    uWS::App().get("/*", [&, &asyncFileStreamer](auto *res, auto *req) {

        /* You can efficiently stream huge files too */
        serveFile(res, req);
        asyncFileStreamer.streamFile(res, req->getUrl());
    }).ws<UserData>("/*", {

            /* Just a few of the available handlers */
            .open = [](auto *ws) {
                std::cout << "A Websocket connected!" << std::endl;
            },
            .message = [&](auto *ws, std::string_view message, uWS::OpCode opCode) {
                std::cout << "Received ws message: " << message << std::endl;
                wss.insert(ws);
                ws->send(message, opCode);
            }

    }).listen(3000, [serving_dir](auto *listenSocket) {

        if (listenSocket) {
            std::cout << "Serving from " << serving_dir << " on port " << 3000 << std::endl;
        }

    }).run();

    std::cout << "Could not start server" << std::endl;
}

void Server::start() {
    std::thread server_t (&Server::server_run, this);
    server_t.detach();
}

void Server::send_string(const std::string &data) const {
    for (auto& ws : wss) {
        ws->send(data, uWS::OpCode::TEXT);
    }
}