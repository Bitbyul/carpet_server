#include "session.h"

using namespace std;
namespace net = boost::asio;

// default constructor
Session::Session() :
        socket_(Manager::get_instance()->get_io_context()),
        receive_buffer_(new unsigned char[kInitialPacketBufferSize]()) {

    Logger::i("Session is created.");
}

Session::~Session() {
    Logger::i("Session is destroyed.");
}
#ifdef CARPET_SERVER
void Session::initialize() {

    // set the no delay option for the socket
    net::ip::tcp::no_delay option(true);
    socket_.set_option(option);

    Logger::i("Session initializing... IP:", socket_.remote_endpoint().address().to_string());

    //unsigned char *send_buffer = new unsigned char[50]();

    //Packet sp(50);
    auto sp = make_shared<Packet>(50);

    sp->set_header(packet_headers::kHELLO);
    sp->write_string(kHELLOMSG);

    //auto buffer_ptr = sp.get_buffer();

    //memcpy(send_buffer, buffer_ptr.get(), sp.size_of_packet());

    //size_t packet_length = sp.size_of_packet();

    send_packet(sp);

    // start an async (non-blocking) write operation to send the packet
    //net::async_write(socket_,
    //                 net::buffer(send_buffer, packet_length),
    //                 std::bind(&Session::after_send,
    //                           shared_from_this(), std::placeholders::_1, std::placeholders::_2, send_buffer));

    // start an async read operation to receive the header of the next packet
    net::async_read(socket_,
                    net::buffer(receive_buffer_.get(), 2),
                    std::bind(&Session::on_receive_data,
                              shared_from_this(), std::placeholders::_1, std::placeholders::_2));

}
#endif
#ifdef CARPET_CLIENT
void Session::connect(boost::asio::ip::tcp::resolver::iterator endpoint_iterator) {
    Logger::i("connecting...");
    // Start an asynchronous connect operation.
    boost::asio::async_connect(socket_, endpoint_iterator,
                               boost::bind(&Session::handle_connect, shared_from_this(),
                                           boost::asio::placeholders::error));
}
#endif

void Session::disconnect() {
    socket_.close();
}
#ifdef CARPET_CLIENT
void Session::handle_connect(const boost::system::error_code& e) {
    Logger::i("handle_connect...");
    if (!e)
    {
        // Successfully established connection. Start operation to read the list
        // of stocks. The connection::async_read() function will automatically
        // decode the data that is read from the underlying socket.
        net::async_read(socket_,
                        net::buffer(receive_buffer_.get(), 2),
                        std::bind(&Session::on_receive_data,
                                  shared_from_this(), std::placeholders::_1, std::placeholders::_2));
    }
    else
    {
        // An error occurred. Log it and return. Since we are not starting a new
        // operation the io_context will run out of work to do and the client will
        // exit.
        std::cerr << e.message() << std::endl;
    }
}
#endif
void Session::after_send(const std::error_code &ec, std::size_t bytes_transferred, shared_ptr<Packet> sp) {
    // free the memory allocated for the send buffer
    //delete[] send_buffer;
    //Logger::i("send OK!");
}

void Session::on_receive_data(const std::error_code &ec, std::size_t bytes_transferred) {
    if (!ec) {
        // get the packet length
        std::uint16_t packet_length = PacketHelper::get_packet_length(receive_buffer_.get());

        //Logger::i("packet length: ", packet_length);

        receive_buffer_.reset(new unsigned char[packet_length]());

        // start an async read operation to receive the rest of the packet
        net::async_read(socket_,
                        net::buffer(receive_buffer_.get(), packet_length),
                        std::bind(&Session::after_receive_data,
                                  shared_from_this(), std::placeholders::_1, std::placeholders::_2));

    }
    else {
        if(ec.value() == 10054 || ec.value() == 104) {
            Logger::i("Client is disconnected.");
            disconnect();

            return;
        }
        Logger::e("on_receive_data ERROR!");
        Logger::e(ec.value(), ":", ec.message());
        disconnect();
    }
}

void Session::after_receive_data(const std::error_code &ec, std::size_t bytes_transferred) {

    if (!ec) {
        Packet rp(receive_buffer_.get(), bytes_transferred);

        Manager::get_instance()->handle_packet(rp, this);
        /*
        if(rp.header() == packet_headers::kPING) {
            Manager::get_instance()->handle_ping(rp, this);
        }else if(rp.header() == packet_headers::kPONG) {
            Manager::get_instance()->handle_pong(rp, this);
        }else if(rp.header() == packet_headers::kCAR_INFO_RESP) {
            Manager::get_instance()->handle_car_info_response(rp, this);
        }
        */
        restart_read();
    } else {
        if(ec.value() == 10054 || ec.value() == 104) {
            Logger::i("Client is disconnected.");
            disconnect();

            return;
        }
        Logger::e("handle_read ERROR!");
        Logger::e(ec.value(), ":", ec.message());
        disconnect();
    }
}

void Session::restart_read() {
    receive_buffer_.reset(new unsigned char[2]());

    // start an async read operation to receive the header of the next packet
    net::async_read(socket_,
                    net::buffer(receive_buffer_.get(), 2),
                    std::bind(&Session::on_receive_data,
                              shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

void Session::send_packet(shared_ptr<Packet> sp) {
    net::async_write(socket_,
                     net::buffer(sp->get_buffer(), sp->size_of_packet()),
                     std::bind(&Session::after_send,
                               shared_from_this(), std::placeholders::_1, std::placeholders::_2, sp));
}

boost::asio::ip::tcp::socket &Session::getSocket() {
    return socket_;
}
