#ifndef CARPET_SESSION_H
#define CARPET_SESSION_H

#include <cstring>
#include <memory>
#include <map>
#include <boost/asio.hpp>

#include "../constants/server_constants.h"
#include "../constants/packet_opcodes.h"
#include "../tools/cplogger.h"
#include "../packet/packet.h"
#include "../packet/packethelper.h"
#include "manager.h"


class Session : public std::enable_shared_from_this<Session> {

    boost::asio::ip::tcp::socket socket_;

    std::unique_ptr<unsigned char> receive_buffer_;

public:

    // default constructor
    Session();

    // copy constructor
    Session(const Session &other) = delete;

    // move constructor
    Session(Session &&other) = delete;

    // destructor
    ~Session();

    // copy assignment operator
    Session &operator=(const Session &other) = delete;

    // move assignment operator
    Session &operator=(Session &&other) = delete;

#ifdef CARPET_SERVER
    void initialize();
#endif
#ifdef CARPET_CLIENT
    void connect(boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
#endif
    void disconnect();

    boost::asio::ip::tcp::socket &getSocket();
#ifdef CARPET_CLIENT
    void handle_connect(const boost::system::error_code& e);
#endif
    void after_send(const std::error_code &ec, size_t bytes_transferred, std::shared_ptr<Packet> sp);
    void on_receive_data(const std::error_code &ec, std::size_t bytes_transferred);
    void after_receive_data(const std::error_code &ec, std::size_t bytes_transferred);

    void restart_read();

    void send_packet(std::shared_ptr<Packet> sp);
};

#endif //CARPET_SESSION_H
