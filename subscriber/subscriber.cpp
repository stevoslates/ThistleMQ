#include <iostream>
#include "subscriber.h"
#include "message.h"

int Subscriber::next_id_ {0};

Subscriber::Subscriber(boost::asio::io_context& io_context, const std::string& topic)
    : sid_(++next_id_), sconn_(io_context), topic_(topic)
{
    if (!connect()) {
        throw std::runtime_error("Failed to connect to broker");
    }
    subscribe(topic);
}

bool Subscriber::connect() {
    boost::system::error_code ec;
    boost::asio::ip::tcp::resolver resolver(sconn_.socket.get_executor());
    
    boost::asio::connect(sconn_.socket, resolver.resolve("127.0.0.1", "8888"), ec);

    if (ec) {
        std::cerr << "Error connecting to broker - check broker exists";
        return false;
    }

    return true;
}

void Subscriber::start() {
    std::cout << "Subscriber: Now Listening!" << "\n";
    listen();
}

void Subscriber::subscribe(const std::string& topic) {
    std::cout << "Subscribing too topic: " << topic << "\n";
    sendmsg(serialize_subscribe(topic));
};

void Subscriber::sendmsg(const std::string& msg) {
    auto result = boost::asio::write(sconn_.socket, boost::asio::buffer(msg));
    std::cout << "Bytes written: " << result << std::endl;
}

void Subscriber::listen() {
    auto self = shared_from_this();
    boost::asio::async_read_until(sconn_.socket, read_buffer_, '\n',
            [this, self](const boost::system::error_code& ec, std::size_t bytes_transferred) {
                if (!ec) {
                    std::istream is(&read_buffer_);
                    std::string data;
                    std::getline(is, data);

                    // Call your deserialization function (assume you already have this):
                    BrokerMessage msg = deserialize_message(data);

                    // Print out to user nicely:
                    std::cout << "[" << msg.topic << "]: " << msg.payload << std::endl;

                    listen();  // Continue reading
                } else {
                    std::cout << "Read error: " << ec.message() << std::endl;
                }
            }
        );
}

Subscriber::~Subscriber() {
    std::cout << "Shutting down Subscriber.." << "\n";
    boost::system::error_code ec;
    sconn_.socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    sconn_.socket.close();
}

