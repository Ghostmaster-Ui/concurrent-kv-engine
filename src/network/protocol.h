#pragma once
#include <string>
#include <vector>

namespace kv {

enum class CommandType : uint8_t {
    GET = 1,
    SET = 2,
    DEL = 3,
    TAG = 4,
    LOOKUP = 5,
    UNKNOWN = 255
};

struct Request {
    CommandType type;
    std::string key;
    std::string value;
    std::vector<std::string> tags;
};

struct Response {
    bool success;
    std::string value;
    std::string message;
    std::vector<std::string> results;
};

class Protocol {
public:
    static std::string encode_request(const Request& req);
    static Request decode_request(const std::string& data);
    static std::string encode_response(const Response& res);
    static Response decode_response(const std::string& data);
};

} // namespace kv
