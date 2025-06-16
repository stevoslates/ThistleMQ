#include <iostream>
#include <sstream>
#include <vector>
#include "broker.h"
#include "message.h"
#include "brokersession.h"

Broker::Broker(boost::asio::io_context& io_context, unsigned short port)
        : acceptor_(io_context,  boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    {
        start_accept();
    }

void Broker::start_accept() {
    acceptor_.async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
                if (!ec) {
                    std::cout << "Client connected!\n";
                    std::cout << "creating session on: " 
                    << socket.remote_endpoint().address().to_string() 
                    << ":" << socket.remote_endpoint().port() << '\n';
                    
                    std::make_shared<BrokerSession>(std::move(socket), *this)->run(); 
                } else {
                    std::cout << "error: " << ec.message() << std::endl;
                }
                start_accept();
            }
        );
}

// TO DO: SHOULD BE ABLE TO SEND SOME SORT OF ACK BACK TO SUBSCRIBER ON FAILURE OR ERROR e.g Subscribed to {topic} succesfully
void Broker::register_subscription(const std::string& topic, std::shared_ptr<BrokerSession> subscriber){
    std::cout << "Registering Subscription!" << "\n";
    auto& subs = topic_subscribers_[topic];
    subs.insert(subscriber);
} 


void Broker::handle_publish(const std::string& topic, const std::string& payload) {
    // Insert topic if it doesn't exist (automatically creates empty set)
    auto& subscribers = topic_subscribers_[topic];
    // Send message to all subscribers (if any)
    for (auto& s : subscribers) {
        s->sendmsg(serialize_broker_message(topic, payload));
    }
}
