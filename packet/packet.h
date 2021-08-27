#ifndef CARPET_PACKET_H
#define CARPET_PACKET_H

#include <cstdint>
#include <string>
#include <memory>

#include "../tools/cplogger.h"

class Packet {

    bool header_set_;
    std::unique_ptr<unsigned char> buffer_;

    uint16_t &data_size_;
    uint16_t &header_;
    unsigned char *body_;

    uint16_t read_pos_;
    uint16_t write_pos_;

    void add_size_of_data(uint16_t size) {
        data_size_ += size;
    }

    void set_size_of_data(uint16_t size) {
        data_size_ = size;
    }

public:

    Packet() = delete;

    Packet(size_t capacity) :
            header_set_(false),
            buffer_(new unsigned char[capacity]()),
            read_pos_(0),
            write_pos_(0),
            data_size_(*(uint16_t*)buffer_.get()),
            header_(*(uint16_t*)(buffer_.get()+2)),
            body_(buffer_.get()+4) {

        set_size_of_data(2); // reserve header 2bytes
        //Logger::i("Packet is created.");
    }

    Packet(unsigned char *buf, uint16_t size_of_data) :
            header_set_(false),
            buffer_(new unsigned char[size_of_data + 2]()),
            read_pos_(0),
            write_pos_(size_of_data-2),
            data_size_(*(uint16_t*)buffer_.get()),
            header_(*(uint16_t*)(buffer_.get()+2)),
            body_(buffer_.get()+4) {

        std::memcpy(&header_, buf, size_of_data);
        set_size_of_data(size_of_data);
        //Logger::i("Packet is created.", header_);
    }

    ~Packet() {
        //Logger::i("Packet is destroyed.", header_);
    }

    uint16_t size_of_body() {
        return data_size_ - 2;
    }

    uint16_t size_of_data() {
        return data_size_;
    }

    uint16_t size_of_packet() {
        return data_size_ + 2;
    }

    uint16_t header(){
        return header_;
    }

    void set_header(std::uint16_t head) {
        header_ = head;
        header_set_ = true;
    }

    bool is_header_set() {
        return header_set_;
    }

    template<typename TYPE>
    void write(TYPE data) {
        *(TYPE*)(body_ + write_pos_) = data;

        write_pos_ += sizeof(TYPE);
        add_size_of_data(sizeof(TYPE));
    }

    void write_string(const std::string& str) {
        //*(uint16_t*)(buffer_.get() + 2 + size_of_data()) = static_cast<uint16_t>(str.size());
        write<uint16_t>(str.size()); // set str size for packet data
        std::copy(str.begin(), str.end(), body_ + write_pos_);

        write_pos_ += str.size();
        add_size_of_data(2 + str.size()); // str size info(2) + str data
    }

    template<typename TYPE>
    TYPE read() {
        TYPE data = *(TYPE*)(body_ + read_pos_);
        read_pos_ += sizeof(TYPE);

        return data;
    }

    std::string read_string() {
        int str_size = read<uint16_t>();
        std::string str((char*)body_ + read_pos_, str_size);
        read_pos_ += str_size;

        return str;
    }

    unsigned char *get_buffer() {
        return buffer_.get();
    }
};


#endif //CARPET_PACKET_H
