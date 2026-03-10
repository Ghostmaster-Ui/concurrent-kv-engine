#include "sharded_map.h"
#include <functional>

namespace kv {

ShardedMap::ShardedMap(size_t num_shards)
    : shards_(num_shards), num_shards_(num_shards) {}

size_t ShardedMap::shard_index(const std::string& key) const {
    return std::hash<std::string>{}(key) % num_shards_;
}

bool ShardedMap::set(const std::string& key, const std::string& value) {
    auto& shard = shards_[shard_index(key)];
    std::unique_lock lock(shard.mutex);
    shard.data[key] = value;
    return true;
}

std::optional<std::string> ShardedMap::get(const std::string& key) const {
    auto& shard = shards_[shard_index(key)];
    std::shared_lock lock(shard.mutex);
    auto it = shard.data.find(key);
    if (it == shard.data.end()) return std::nullopt;
    return it->second;
}

bool ShardedMap::del(const std::string& key) {
    auto& shard = shards_[shard_index(key)];
    std::unique_lock lock(shard.mutex);
    return shard.data.erase(key) > 0;
}

size_t ShardedMap::size() const {
    size_t total = 0;
    for (auto& shard : shards_) {
        std::shared_lock lock(shard.mutex);
        total += shard.data.size();
    }
    return total;
}

} // namespace kv
