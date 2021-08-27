#ifndef CARPET_CLIENT_PACKET_OPCODES_H
#define CARPET_CLIENT_PACKET_OPCODES_H
#include <cstdint>

namespace packet_headers {
    constexpr std::uint16_t kHELLO = 0x00;
    constexpr std::uint16_t kPING = 0x01;
    constexpr std::uint16_t kPONG = 0x02;
    constexpr std::uint16_t kCAR_INFO_REQ = 0x10;
    constexpr std::uint16_t kCAR_INFO_RESP = 0x11;
}

#endif //CARPET_CLIENT_PACKET_OPCODES_H
