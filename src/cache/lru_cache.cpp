#include "lru_cache.h"

namespace kv {

LRUCache::LRUCache(size_t capacity) : capacity_(capacity) {}

void LRUCache::put(const std::string& key, const std::string& value) {
    std::lock_guard lock(mutex_);
    auto it = map_.find(key);
    if (it != map_.end()) {
        order_.erase(it->second);
        map_.erase(it);
    }
    if (order_.size() >= capacity_) {
        map_.erase(order_.back().first);
        order_.pop_back();
    }
    order_.push_front({key, value});
    map_[key] = order_.begin();
}

std::optional<std::string> LRUCache::get(const std::string& key) {
    std::lock_guard lock(mutex_);
    auto it = map_.find(key);
    if (it == map_.end()) return std::nullopt;
    order_.splice(order_.begin(), order_, it->second);
    return it->second->second;
}

void LRUCache::invalidate(const std::string& key) {
    std::lock_guard lock(mutex_);
    auto it = map_.find(key);
    if (it == map_.end()) return;
    order_.erase(it->second);
    map_.erase(it);
}

size_t LRUCache::size() const {
    std::lock_guard lock(mutex_);
    return map_.size();
}

} // namespace kv
