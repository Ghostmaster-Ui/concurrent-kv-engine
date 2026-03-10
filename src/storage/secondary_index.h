#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <shared_mutex>
#include <vector>

namespace kv {

class SecondaryIndex {
public:
    void add(const std::string& tag, const std::string& key);
    void remove(const std::string& tag, const std::string& key);
    std::vector<std::string> lookup(const std::string& tag) const;
    void remove_key(const std::string& key);

private:
    std::unordered_map<std::string, std::unordered_set<std::string>> index_;
    mutable std::shared_mutex mutex_;
};

} // namespace kv
