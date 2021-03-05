//
// Created by jm1417 on 04/03/2021.
//

#include <uWebSockets/App.h>
#include <filesystem>
#include <iostream>
#include "simulator/util/utility.h"
#include "simulator/ui/ui.h"
#include "simulator/ui/async_file_streamer.h"
#include "simulator/ui/file_watcher.h"
#include "simulator/ui/base64_encoder.h"
#include <thread>
#include <vector>
#include <opencv2/imgcodecs.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string file_path(const char *path) {
    std::string path_s = path;
    return path_s.substr(0, path_s.find_last_of("\\/"));
}

void UI::server_run() {
    std::string serving_dir = file_path(__FILE__) + std::filesystem::path::preferred_separator + "src";

    AsyncFileStreamer asyncFileStreamer(serving_dir);
    FileWatcher fw{serving_dir, std::chrono::milliseconds(3000)};

    std::thread filewatcher_t([&fw, &asyncFileStreamer] {
        fw.start([&asyncFileStreamer](const std::string& path_to_watch, FileStatus status) -> void {
            // Process only regular files, all other file types are ignored
            if (!std::filesystem::is_regular_file(std::filesystem::path(path_to_watch)) && status != FileStatus::erased) {
                return;
            }
            std::cout << "Updating files" << std::endl;
            asyncFileStreamer.updateRootCache();
        });
    });
    filewatcher_t.detach();


    uWS::App().get("/*", [this, &asyncFileStreamer](auto *res, auto *req) {

        serveFile(res, req);
        asyncFileStreamer.streamFile(res, req->getUrl());
    }).ws<UserData>("/*", {

            .open = [&](auto *ws) {
                std::cout << "A Websocket connected!" << std::endl;
                wss.insert(ws);
            },
            .message = [](auto *ws, std::string_view message, uWS::OpCode opCode) {
                ws->send(message, opCode);
            }

    }).listen(3000, [serving_dir](auto *listenSocket) {

        if (listenSocket) {
            std::cout << "Serving from " << serving_dir << " on port " << 3000 << std::endl;
        }

    }).run();

    std::cout << "Could not start server" << std::endl;
}

void UI::start() {
    std::thread server_t(&UI::server_run, this);
    server_t.detach();
}

void UI::send_string(const std::string &data) const {
    for (auto &ws : wss) {
        ws->send(data, uWS::OpCode::TEXT);
    }
}

void UI::display_reg(Register& reg) {
    if (wss.empty()) return;
    cv::Mat remapped;
    utility::remap_image(reg, remapped);
    std::vector<uchar> buf;
    cv::imencode(".jpg", remapped, buf);
    std::string out = base64::encode(&buf[0], buf.size());
    json j;
    j["reg"] =  reg.name_;
    j["data"] = out;
    for (auto &ws : wss) {
        ws->send(j.dump(0), uWS::OpCode::TEXT);
    }
}
