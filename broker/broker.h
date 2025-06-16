#ifndef BROKER_H
#define BROKER_H

#include <boost/asio.hpp>
#include <unordered_set>


class BrokerSession;


class Broker {

public:
    Broker(boost::asio::io_context& io_context, unsigned short port);

    void register_subscription(const std::string& topic, std::shared_ptr<BrokerSession> subscriber);
    void handle_publish(const std::string& topic, const std::string& payload);

private:
    void start_accept();

    boost::asio::ip::tcp::acceptor acceptor_;
    std::unordered_map<std::string, std::unordered_set<std::shared_ptr<BrokerSession>>> topic_subscribers_;

};


#endif 