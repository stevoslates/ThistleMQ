#include <iostream>
#include "brokersession.h"
#include "message.h"


BrokerSession::BrokerSession(boost::asio::ip::tcp::socket socket, Broker& broker)  
    : s_socket(std::move(socket)), s_broker(broker) { }


void BrokerSession::run() {
        wait_for_request();
    }

void BrokerSession::wait_for_request() {
    // since we capture `this` in the callback, we need to call shared_from_this()
    auto self(shared_from_this());
        
    boost::asio::async_read_until(s_socket, s_buffer, "\n", //currently null temrinator but change
    [this, self](boost::system::error_code ec, std::size_t /*length*/)
    {
        if (!ec)  {
            std::istream is(&s_buffer);
            std::string data;
            std::getline(is, data);
                
            //can delete just prints the data
            std::cout << "Packet Recieved: " << data << std::endl;

            //EXTRACT PACKET TYPE + ROUTING
            handle_incoming_message(data);
                
            wait_for_request();
        } else if (ec == boost::asio::error::eof) {
            std::cout << "Client disconnected." << std::endl; //for the broker do we want this? we want a client to have the connection open -> think!
        } else {
            std::cout << "Error: " << ec.message() << std::endl;
        }
    });
}

void BrokerSession::handle_incoming_message(std::string& data) {
    BrokerMessage msg {deserialize_message(data)};

    switch (msg.type) {
        case MessageType::PUBLISH: {
            std::cout << "Handling PUBLISH" << std::endl;
            s_broker.handle_publish(msg.topic, msg.payload);
            break;
        }

        case MessageType::SUBSCRIBE: {
            std::cout << "Handling SUBSCRIBE" << std::endl;
            s_broker.register_subscription(msg.topic, shared_from_this());
            break;
        }

        case MessageType::UNKNOWN:
        default: {
            std::cout << "Handling UNKNOWN" << std::endl;
            break;
        }
    }
}

//Gets called via Broker
void BrokerSession::sendmsg(const std::string& msg) {
    auto self = shared_from_this();  // ensure session stays alive during async
    auto buffer = std::make_shared<std::string>(msg);
    
    write_queue_.push_back(buffer);

    // Only start async_write if no other write is in progress
    if (write_queue_.size() == 1) {
        do_write();
    }
    
}

void BrokerSession::do_write() {
    auto self = shared_from_this();

    boost::asio::async_write(
        s_socket,
        boost::asio::buffer(*write_queue_.front()),
        [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
            if (!ec) {
                write_queue_.pop_front();
                if (!write_queue_.empty()) {
                    do_write();
                }
            } else {
                std::cout << "Write error: " << ec.message() << std::endl;
            }
        }
    );
}