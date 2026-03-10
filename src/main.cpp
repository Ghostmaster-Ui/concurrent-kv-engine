#include <iostream>
#include <vector>
#include "storage/storage_engine.h"
#include "cache/lru_cache.h"
#include "network/server.h"

int main() {
    // Test storage engine
    std::cout << "--- Storage Engine ---" << std::endl;
    kv::StorageEngine engine(64);

    engine.set("user:1", "Advit", {"users", "active"});
    engine.set("user:2", "Singh", {"users"});
    engine.set("project:1", "kv-engine", {"projects"});

    auto val = engine.get("user:1");
    if (val) std::cout << "user:1 = " << *val << std::endl;

    auto keys = engine.lookup_by_tag("users");
    std::cout << "tag:users -> ";
    for (const auto& k : keys) std::cout << k << " ";
    std::cout << std::endl;

    engine.del("user:2");
    std::cout << "Engine size: " << engine.size() << std::endl;

    // Test LRU cache
    std::cout << "\n--- LRU Cache ---" << std::endl;
    kv::LRUCache cache(3);
    cache.put("a", "1");
    cache.put("b", "2");
    cache.put("c", "3");

    auto v = cache.get("a");
    if (v) std::cout << "cache get a = " << *v << std::endl;

    cache.put("d", "4");
    auto evicted = cache.get("b");
    std::cout << "b after eviction: " << (evicted ? *evicted : "NOT FOUND") << std::endl;

    // Start server
    std::cout << "\n--- Starting KV Server ---" << std::endl;
    kv::Server server(6380, 10000);
    server.start();

    return 0;
}
