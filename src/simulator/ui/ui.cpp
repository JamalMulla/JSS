//
// Created by jm1417 on 04/03/2021.
//

#include "simulator/ui/ui.h"

#include <uWebSockets/App.h>
#include <rttr/registration>
#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <string>
#include <thread>
#include <vector>

#include "simulator/ui/async_file_streamer.h"
#include "simulator/ui/base64_encoder.h"
#include "simulator/ui/file_watcher.h"
#include "simulator/util/utility.h"

using json = nlohmann::ordered_json;

std::string file_path(const char *path) {
    std::string path_s = path;
    return path_s.substr(0, path_s.find_last_of("\\/"));
}

void UI::server_run() {
    std::string serving_dir = file_path(__FILE__) + "/src";

    AsyncFileStreamer asyncFileStreamer(serving_dir);
#ifndef NDEBUG
    FileWatcher fw {serving_dir, std::chrono::milliseconds(3000)};

    std::thread filewatcher_t([&fw, &asyncFileStreamer] {
      fw.start([&asyncFileStreamer](const std::string &path_to_watch,
                                    FileStatus status) -> void {
        // Process only regular files, all other file types are ignored
        if (!std::filesystem::is_regular_file(
            std::filesystem::path(path_to_watch)) &&
            status != FileStatus::erased) {
            return;
        }
        std::cout << "Updating files" << std::endl;
        asyncFileStreamer.updateRootCache();
      });
    });
    filewatcher_t.detach();
#endif

    uWS::App()
        .get("/*",
             [this, &asyncFileStreamer](auto *res, auto *req) {
                 serveFile(res, req);
                 asyncFileStreamer.streamFile(res, req->getUrl());
             })
        .ws<UserData>(
            "/*",
            {

                .open =
                    [&](auto *ws) {
                        std::cout << "A Websocket connected!" << std::endl;
                        wss.insert(ws);
                    },
                .message = [](auto *ws, std::string_view message,
                              uWS::OpCode opCode) { ws->send(message, opCode); }

            })
        .listen(3000,
                [serving_dir](auto *listenSocket) {
                    if (listenSocket) {
                        std::cout << "Serving from " << serving_dir
                                  << " on port " << 3000 << std::endl;
                    }
                })
        .run();

    this->has_started = false;
    std::cerr << "Could not start server" << std::endl;
}

void UI::start() {
    if (!this->has_started) {
        this->has_started = true;
        std::thread server_t(&UI::server_run, this);
        server_t.detach();
    }
}

void UI::send_string(const std::string &data) const {
    if (!this->has_started) {
        std::cerr << "UI hasn't been started. Call start() first" << std::endl;
        return;
    }
    for (auto &ws: wss) { ws->send(data, uWS::OpCode::TEXT); }
}

void UI::display_reg(std::shared_ptr<Register> reg) {
    if (!this->has_started) {
        std::cerr << "UI hasn't been started. Call start() first" << std::endl;
        return;
    }
    if (wss.empty()) {
        return;
    }
    cv::Mat remapped;
    utility::remap_register(*reg, remapped);
    std::vector<uchar> buf;
    cv::imencode(".jpg", remapped, buf);
    std::string out = base64::encode(&buf[0], buf.size());
    json j;
    j["reg"] = reg->name_;
    j["data"] = out;
    send_string(j.dump(0));
}

RTTR_REGISTRATION {
    using namespace rttr;

    registration::class_<UI>("UI")
        .constructor<>()
        .method("display_reg", &UI::display_reg)
        .method("start", &UI::start);
};
