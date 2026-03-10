#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>

namespace kv {

Server::Server(int port, size_t cache_size)
    : port_(port), server_fd_(-1), running_(false),
      engine_(64), cache_(cache_size) {}

Server::~Server() { stop(); }

void Server::start() {
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ < 0)
        throw std::runtime_error("Failed to create socket");

    int opt = 1;
    setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port_);

    if (bind(server_fd_, (sockaddr*)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("Failed to bind");

    if (listen(server_fd_, 256) < 0)
        throw std::runtime_error("Failed to listen");

    running_ = true;
    std::cout << "KV Server listening on port " << port_ << std::endl;
    accept_loop();
}

void Server::stop() {
    running_ = false;
    if (server_fd_ >= 0) {
        close(server_fd_);
        server_fd_ = -1;
    }
}

void Server::accept_loop() {
    while (running_) {
        int client_fd = accept(server_fd_, nullptr, nullptr);
        if (client_fd < 0) continue;
        std::thread([this, client_fd]() {
            handle_client(client_fd);
        }).detach();
    }
}

void Server::handle_client(int client_fd) {
    char buf[4096];
    while (true) {
        int n = read(client_fd, buf, sizeof(buf));
        if (n <= 0) break;
        try {
            Request req = Protocol::decode_request(std::string(buf, n));
            Response res = process_request(req);
            std::string encoded = Protocol::encode_response(res);
            write(client_fd, encoded.c_str(), encoded.size());
        } catch (...) { break; }
    }
    close(client_fd);
}

Response Server::process_request(const Request& req) {
    Response res;
    switch (req.type) {
        case CommandType::GET: {
            auto cached = cache_.get(req.key);
            if (cached) {
                res.success = true;
                res.value = *cached;
                res.message = "OK (cache)";
            } else {
                auto val = engine_.get(req.key);
                if (val) {
                    cache_.put(req.key, *val);
                    res.success = true;
                    res.value = *val;
                    res.message = "OK";
                } else {
                    res.success = false;
                    res.message = "NOT FOUND";
                }
            }
            break;
        }
        case CommandType::SET:
            cache_.invalidate(req.key);
            res.success = engine_.set(req.key, req.value);
            res.message = "OK";
            break;
        case CommandType::DEL:
            cache_.invalidate(req.key);
            res.success = engine_.del(req.key);
            res.message = res.success ? "OK" : "NOT FOUND";
            break;
        default:
            res.success = false;
            res.message = "UNKNOWN COMMAND";
    }
    return res;
}

} // namespace kv
