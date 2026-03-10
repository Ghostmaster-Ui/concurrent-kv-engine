#pragma once
#include <atomic>
#include <thread>
#include "storage/storage_engine.h"
#include "cache/lru_cache.h"
#include "network/protocol.h"

namespace kv {

class Server {
public:
    Server(int port, size_t cache_size = 10000);
    ~Server();

    void start();
    void stop();

private:
    int port_;
    int server_fd_;
    std::atomic<bool> running_;
    StorageEngine engine_;
    LRUCache cache_;

    void accept_loop();
    void handle_client(int client_fd);
    Response process_request(const Request& req);
};

} // namespace kv
