#pragma once
#include <string>
#include <vector>
#include <optional>
#include "sharded_map.h"
#include "secondary_index.h"

namespace kv {

class StorageEngine {
public:
    explicit StorageEngine(size_t num_shards = 64);

    bool set(const std::string& key, const std::string& value,
             const std::vector<std::string>& tags = {});
    std::optional<std::string> get(const std::string& key);
    bool del(const std::string& key);
    std::vector<std::string> lookup_by_tag(const std::string& tag) const;
    size_t size() const;

private:
    ShardedMap map_;
    SecondaryIndex index_;
};

} // namespace kv
