#include "secondary_index.h"

namespace kv {

void SecondaryIndex::add(const std::string& tag, const std::string& key) {
    std::unique_lock lock(mutex_);
    index_[tag].insert(key);
}

void SecondaryIndex::remove(const std::string& tag, const std::string& key) {
    std::unique_lock lock(mutex_);
    auto it = index_.find(tag);
    if (it != index_.end())
        it->second.erase(key);
}

std::vector<std::string> SecondaryIndex::lookup(const std::string& tag) const {
    std::shared_lock lock(mutex_);
    auto it = index_.find(tag);
    if (it == index_.end()) return {};
    return std::vector<std::string>(it->second.begin(), it->second.end());
}

void SecondaryIndex::remove_key(const std::string& key) {
    std::unique_lock lock(mutex_);
    for (auto& [tag, keys] : index_)
        keys.erase(key);
}

} // namespace kv
