#include "storage_engine.h"

namespace kv {

StorageEngine::StorageEngine(size_t num_shards)
    : map_(num_shards) {}

bool StorageEngine::set(const std::string& key, const std::string& value,
                        const std::vector<std::string>& tags) {
    bool ok = map_.set(key, value);
    for (const auto& tag : tags)
        index_.add(tag, key);
    return ok;
}

std::optional<std::string> StorageEngine::get(const std::string& key) {
    return map_.get(key);
}

bool StorageEngine::del(const std::string& key) {
    index_.remove_key(key);
    return map_.del(key);
}

std::vector<std::string> StorageEngine::lookup_by_tag(const std::string& tag) const {
    return index_.lookup(tag);
}

size_t StorageEngine::size() const {
    return map_.size();
}

} // namespace kv
