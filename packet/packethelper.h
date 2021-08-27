#ifndef CARPET_PACKETHELPER_H
#define CARPET_PACKETHELPER_H

#include <cstdint>

class PacketHelper {
public:
    static std::uint16_t get_packet_length(unsigned char *buffer);

};


#endif //CARPET_PACKETHELPER_H
