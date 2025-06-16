#include <boost/asio.hpp>
#include <iostream>
#include "publisher.h"
#include "message.h"


Publisher::Publisher(boost::asio::io_context& io_context)
        : pid_(++next_id_), pconn_(io_context)
    {
        if (!connect()) {
            throw std::runtime_error("Failed to connect to broker");
        }
    }

bool Publisher::connect() {
    boost::system::error_code ec;
    boost::asio::ip::tcp::resolver resolver(pconn_.socket.get_executor());
    
    boost::asio::connect(pconn_.socket, resolver.resolve("127.0.0.1", "8888"), ec);

    if (ec) {
        std::cerr << "Error connecting to broker - check broker exists";
        return false;
    }
    return true;
}

//TO DO: MAKE THIS A ASYNC WRITE
void Publisher::sendmsg(const std::string& topic, const std::string& msg) {
   //std::string data {seralizemsg(topic, msg)};
   std::string data = serialize_publish(topic, msg);  // Use protocol function
   auto result = boost::asio::write(pconn_.socket, boost::asio::buffer(data));
   std::cout << "Bytes written: " << result << std::endl;
}


Publisher::~Publisher() {
    std::cout << "Shutting down Publisher.." << "\n";
    boost::system::error_code ec;
    pconn_.socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    pconn_.socket.close();
}

int Publisher::next_id_ = 0;