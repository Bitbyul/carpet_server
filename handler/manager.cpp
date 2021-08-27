#include "manager.h"

using namespace std;
namespace net = boost::asio;

Manager *Manager::instance_ = nullptr;
uint64_t Manager::ping_sent_time_ = 0;

Manager::Manager(
#ifdef CARPET_SERVER
        bool dedicated_server
#endif
)
#ifdef CARPET_SERVER
        :is_dedicated_server_(dedicated_server), is_accepting_connections_(false)
#endif
{
    instance_ = this;

#ifdef CARPET_SERVER
    register_handler(packet_headers::kPING, handle_ping);
    register_handler(packet_headers::kPONG, handle_pong);
    register_handler(packet_headers::kCAR_INFO_RESP, handle_car_info_response);

    is_accepting_connections_ = true;
#endif
#ifdef CARPET_CLIENT
    register_handler(packet_headers::kHELLO, handle_hello);
    register_handler(packet_headers::kPING, handle_ping);
    register_handler(packet_headers::kPONG, handle_pong);
    register_handler(packet_headers::kCAR_INFO_REQ, handle_car_info_request);
#endif

    Logger::i("Connection manager is created.");

}

Manager* Manager::get_instance() {
    return instance_;
}

boost::asio::io_context &Manager::get_io_context() {
    return io_context_;
}
#ifdef CARPET_CLIENT
void Manager::connect_to_server() {

    shared_ptr<Session> session(new Session());

    boost::asio::io_context &io = io_context_;
    boost::asio::io_context::work work(io);

    // Resolve the host name into an IP address.
    net::ip::tcp::resolver resolver(io);
    net::ip::tcp::resolver::query query("skystar.kr", "7143");
    net::ip::tcp::resolver::iterator endpoint_iterator =
            resolver.resolve(query);

    session->connect(endpoint_iterator);

    io.run();
}
#endif
#ifdef CARPET_SERVER
bool Manager::is_dedicated_server() {
    return is_dedicated_server_;
}

bool Manager::is_accepting_connections() {
    return is_accepting_connections_;
}

void Manager::wait_and_accept_client(){
    auto endpoint = net::ip::tcp::endpoint(net::ip::tcp::v4(), kServerPort);
    net::ip::tcp::acceptor acceptor(io_context_, endpoint);

    while(is_accepting_connections()){
        shared_ptr<Session> session(new Session());

        error_code ec;
        acceptor.accept(session->getSocket());

        if(!ec){
            session->initialize();
        }
    }
}

void Manager::start_listen_for(){
    std::thread listen_thread(&Manager::wait_and_accept_client, this);

    boost::asio::io_context::work work(io_context_);

    for(;;) {
        io_context_.run();
    }
}
#endif
template<typename Func>
bool Manager::register_handler(std::uint16_t header, Func handler) {
    handler_map_.insert(make_pair(header, handler));

    return true;
}

void Manager::handle_packet(Packet &rp, Session *session) {

    //Logger::i("size_of_data:", rp.size_of_data());

    auto mapped_func = handler_map_.find(rp.header());
    if(mapped_func != handler_map_.end()) mapped_func->second(rp, session);
    else Logger::e("UNKNOWN HEADER received:", rp.header());
}
#ifdef CARPET_SERVER
void Manager::handle_ping(Packet& rp, Session *session) {

    Logger::i("kPING received.");

    auto sp = make_shared<Packet>(50);
    sp->set_header(packet_headers::kPONG);
    sp->write(0);

    session->send_packet(sp);

    ping_sent_time_ = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

    auto sp2 = make_shared<Packet>(50);
    sp2->set_header(packet_headers::kPING);
    sp2->write<uint16_t>(0);

    session->send_packet(sp2);

}
void Manager::handle_pong(Packet& rp, Session *session) {

    Logger::i("kPONG received.");

    int64_t now = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

    Logger::i("Server->Client->Server: ", now - ping_sent_time_, "mills.");

}
void Manager::handle_car_info_response(Packet& rp, Session *session) {
    Logger::i("kCAR_INFO_RESP");
    Logger::i("data1:",rp.read<uint32_t>());
    Logger::i("data2:",rp.read<uint32_t>());

    std::this_thread::sleep_for(std::chrono::microseconds(1000));

    auto sp = make_shared<Packet>(50);
    sp->set_header(packet_headers::kPING);
    sp->write<uint16_t>(0);

    session->send_packet(sp);

}
#endif
#ifdef CARPET_CLIENT
void Manager::handle_hello(Packet& rp, Session *session) {

    Logger::i("kHello received.");
    string hello_msg(rp.read_string());
    Logger::i("Hello msg: ", hello_msg);
    if (hello_msg != kHELLOMSG) {
        Logger::e("Hello msg mismatch. terminating the connection...");
    }

    auto sp = make_shared<Packet>(50);
    sp->set_header(packet_headers::kCAR_INFO_RESP);
    sp->write<uint32_t>(970718);
    sp->write<uint32_t>(1500015);

    session->send_packet(sp);

}
void Manager::handle_ping(Packet& rp, Session *session) {

    Logger::i("kPING received.");

    auto sp = make_shared<Packet>(50);
    sp->set_header(packet_headers::kPONG);
    sp->write(0);

    session->send_packet(sp);

    ping_sent_time_ = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

    auto sp2 = make_shared<Packet>(50);
    sp2->set_header(packet_headers::kPING);
    sp2->write<uint16_t>(0);

    session->send_packet(sp2);

}
void Manager::handle_pong(Packet& rp, Session *session) {

    Logger::i("kPONG received.");

    int64_t now = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

    Logger::i("Client->Server->Client: ", now - ping_sent_time_, "mills.");

}
void Manager::handle_car_info_request(Packet& rp, Session *session) {

    Logger::i("kCAR_INFO_REQ received.");

    auto sp = make_shared<Packet>(50);
    sp->set_header(packet_headers::kCAR_INFO_RESP);
    sp->write<uint32_t>(970718);
    sp->write<uint32_t>(1500015);

    session->send_packet(sp);

}
#endif