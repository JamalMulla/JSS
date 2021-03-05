//
// Created by jm1417 on 04/03/2021.
//

#ifndef SIMULATOR_SERVER_H
#define SIMULATOR_SERVER_H


#include <string>
#include <set>
#include <opencv2/core/mat.hpp>
#include <uWebSockets/App.h>
#include <uWebSockets/WebSocket.h>

struct UserData {
    std::string username;
};


class Server{
private:
    std::set<uWS::WebSocket<false, true>*> wss;

    /* Middleware to fill out content-type */
    static inline bool hasExt(std::string_view file, std::string_view ext) {
        if (ext.size() > file.size()) {
            return false;
        }
        return std::equal(ext.rbegin(), ext.rend(), file.rbegin());
    }

    /* This should be a filter / middleware like app.use(handler) */
    template <bool SSL>
    uWS::HttpResponse<SSL> *serveFile(uWS::HttpResponse<SSL> *res, uWS::HttpRequest *req) {
        res->writeStatus(uWS::HTTP_200_OK);

        if (hasExt(req->getUrl(), ".html")) {
            res->writeHeader("Content-Type", "text/html");
        } else if (hasExt(req->getUrl(), ".js")) {
            res->writeHeader("Content-Type", "text/javascript");
        } else if (hasExt(req->getUrl(), ".svg")) {
            res->writeHeader("Content-Type", "image/svg+xml");
        }

        return res;
    }

    void server_run();

public:
    void start();
    void send_string(const std::string& data) const;
    void display_reg(Register& reg);
};





#endif //SIMULATOR_SERVER_H
