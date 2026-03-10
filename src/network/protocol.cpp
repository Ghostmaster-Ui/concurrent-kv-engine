#include "protocol.h"
#include <stdexcept>

namespace kv {

std::string Protocol::encode_request(const Request& req) {
    std::string data;
    data += static_cast<char>(req.type);

    uint16_t kl = req.key.size();
    data += static_cast<char>(kl >> 8);
    data += static_cast<char>(kl & 0xFF);
    data += req.key;

    uint16_t vl = req.value.size();
    data += static_cast<char>(vl >> 8);
    data += static_cast<char>(vl & 0xFF);
    data += req.value;

    return data;
}

Request Protocol::decode_request(const std::string& data) {
    if (data.size() < 5)
        throw std::runtime_error("Request too short");

    Request req;
    req.type = static_cast<CommandType>(data[0]);

    uint16_t kl = (static_cast<uint8_t>(data[1]) << 8)
                 | static_cast<uint8_t>(data[2]);
    req.key = data.substr(3, kl);

    uint16_t vl = (static_cast<uint8_t>(data[3 + kl]) << 8)
                 | static_cast<uint8_t>(data[4 + kl]);
    req.value = data.substr(5 + kl, vl);

    return req;
}

std::string Protocol::encode_response(const Response& res) {
    std::string data;
    data += static_cast<char>(res.success ? 1 : 0);

    uint16_t vl = res.value.size();
    data += static_cast<char>(vl >> 8);
    data += static_cast<char>(vl & 0xFF);
    data += res.value;

    uint16_t ml = res.message.size();
    data += static_cast<char>(ml >> 8);
    data += static_cast<char>(ml & 0xFF);
    data += res.message;

    return data;
}

Response Protocol::decode_response(const std::string& data) {
    if (data.size() < 5)
        throw std::runtime_error("Response too short");

    Response res;
    res.success = data[0] == 1;

    uint16_t vl = (static_cast<uint8_t>(data[1]) << 8)
                 | static_cast<uint8_t>(data[2]);
    res.value = data.substr(3, vl);

    uint16_t ml = (static_cast<uint8_t>(data[3 + vl]) << 8)
                 | static_cast<uint8_t>(data[4 + vl]);
    res.message = data.substr(5 + vl, ml);

    return res;
}

} // namespace kv
