cmake_minimum_required(VERSION 3.16)
project(ConcurrentKVEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Threads REQUIRED)

add_executable(kv_server
    src/main.cpp
    src/storage/sharded_map.cpp
    src/storage/secondary_index.cpp
    src/storage/storage_engine.cpp
    src/cache/lru_cache.cpp
    src/network/server.cpp
    src/network/protocol.cpp
)

target_include_directories(kv_server PRIVATE src)
target_link_libraries(kv_server Threads::Threads)
