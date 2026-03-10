#pragma once
#include <string>
#include <unordered_map>
#include <shared_mutex>
#include <vector>
#include <optional>

namespace kv {

struct Shard {
    std::unordered_map<std::string, std::string> data;
    mutable std::shared_mutex mutex;
};

class ShardedMap {
public:
    explicit ShardedMap(size_t num_shards = 64);

    bool set(const std::string& key, const std::string& value);
    std::optional<std::string> get(const std::string& key) const;
    bool del(const std::string& key);
    size_t size() const;

private:
    std::vector<Shard> shards_;
    size_t num_shards_;

    size_t shard_index(const std::string& key) const;
};

} // namespace kv
