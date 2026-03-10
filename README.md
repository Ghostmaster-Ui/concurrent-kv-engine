# Concurrent Key-Value Storage Engine

A fast, thread-safe storage engine built from scratch in C++17, supporting multiple concurrent readers and writers without data corruption.

## Performance
- **159,000+ ops/sec** across 200 concurrent connections
- **6ms** read latency with cache (down from 45ms cold)
- **Zero data corruption** validated under heavy concurrent load

## Architecture

### Core Systems
- **Sharded hashmap** — 64 shards with fine-grained reader-writer locks (`shared_mutex`), true parallel reads
- **Secondary index** — tag-based lookups, map keys to arbitrary tags for fast group queries
- **LRU cache layer** — sits in front of storage engine, cuts read latency from 45ms to 6ms
- **TCP server** — handles 200+ concurrent connections via detached threads on port 6380
- **Custom binary protocol** — length-prefixed frames, zero text parsing overhead

### Project Structure
src/
├── storage/
│   ├── sharded_map      # 64-shard hashmap with shared_mutex per shard
│   ├── secondary_index  # Tag-based key grouping and lookup
│   └── storage_engine   # Unified interface over map + index
├── cache/
│   └── lru_cache        # O(1) LRU cache with mutex protection
└── network/
├── server           # TCP server, detached client threads
└── protocol         # Binary wire protocol encoder/decoder

## Build & Run

```bash
mkdir build && cd build
cmake ..
make
./kv_server
```

## Benchmark

```bash
c++ -std=c++17 -O2 -pthread benchmarks/throughput_test.cpp -o bench
./bench
```

## Tech Stack
- C++17
- std::shared_mutex (reader-writer locks)
- POSIX sockets (TCP)
- std::thread, std::atomic
- CMake
