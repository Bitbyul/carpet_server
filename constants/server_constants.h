#ifndef CARPET_SERVER_SERVER_CONSTANTS_H
#define CARPET_SERVER_SERVER_CONSTANTS_H

constexpr unsigned short kServerPort = 7143;

constexpr std::size_t kPacketMaxBufferLength = 65535;
constexpr std::size_t kIvBufferSize = 16;
constexpr std::size_t kInitialPacketBufferSize = 600;
constexpr std::size_t kIncreasePacketBufferSize = 100;

const std::string kHELLOMSG = "!^cpmagicstr0797|0.1";

#endif //CARPET_SERVER_SERVER_CONSTANTS_H
