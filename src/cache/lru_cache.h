#pragma once
#include <string>
#include <list>
#include <unordered_map>
#include <optional>
#include <mutex>

namespace kv {

class LRUCache {
public:
    explicit LRUCache(size_t capacity);

    void put(const std::string& key, const std::string& value);
    std::optional<std::string> get(const std::string& key);
    void invalidate(const std::string& key);
    size_t size() const;

private:
    size_t capacity_;
    std::list<std::pair<std::string, std::string>> order_;
    std::unordered_map<std::string,
        std::list<std::pair<std::string, std::string>>::iterator> map_;
    mutable std::mutex mutex_;
};

} // namespace kv
