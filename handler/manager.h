#ifndef CARPET_MANAGER_H
#define CARPET_MANAGER_H

#include <thread>
#include <chrono>
#include <boost/asio.hpp>

#include "../constants/server_constants.h"
#include "../constants/packet_opcodes.h"
#include "../tools/cplogger.h"
#include "../handler/session.h"

class Session;

class Manager {

    static Manager *instance_; // for singleton pattern

#ifdef CARPET_SERVER
    bool is_dedicated_server_;
    bool is_accepting_connections_;
#endif

    boost::asio::io_context io_context_;

    std::map<uint16_t, std::function<void(Packet&, Session*)>> handler_map_;

    static uint64_t ping_sent_time_;

public:
    // constructor
#ifdef CARPET_SERVER
    Manager() = delete;
#endif
    Manager(
#ifdef CARPET_SERVER
            bool dedicated_server
#endif
    );

    // copy constructor
    Manager(const Manager &other) = default;

    // move constructor
    Manager(Manager &&other) = default;

    // destructor
    ~Manager() = default;

    // copy assignment operator
    Manager &operator=(const Manager &other) = default;

    // move assignment operator
    Manager &operator=(Manager &&other) = default;

    static Manager *get_instance();

#ifdef CARPET_CLIENT
    void connect_to_server();
#endif
#ifdef CARPET_SERVER
    void wait_and_accept_client();
    void start_listen_for();

    bool is_dedicated_server();
    bool is_accepting_connections();
#endif

    boost::asio::io_context &get_io_context();

    template<typename Func>
    bool register_handler(std::uint16_t header, Func handler);

    void handle_packet(Packet &rp, Session *session);

#ifdef CARPET_CLIENT
    static void handle_hello(Packet& rp, Session *session);
    static void handle_ping(Packet& rp, Session *session);
    static void handle_pong(Packet& rp, Session *session);
    static void handle_car_info_request(Packet& rp, Session *session);
#endif
#ifdef CARPET_SERVER
    static void handle_ping(Packet& rp, Session *session);
    static void handle_pong(Packet& rp, Session *session);
    static void handle_car_info_response(Packet& rp, Session *session);
#endif
};


#endif //CARPET_MANAGER_H
