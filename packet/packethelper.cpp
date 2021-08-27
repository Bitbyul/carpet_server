#include "packethelper.h"

std::uint16_t PacketHelper::get_packet_length(unsigned char *buffer) {
    return *(std::uint16_t *)(buffer);
}