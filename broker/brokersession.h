#ifndef BROKERSESSION_H
#define BROKERSESSION_H

#include <boost/asio.hpp>
#include "broker.h"


class BrokerSession : public std::enable_shared_from_this<BrokerSession> {
public:
    //constructor must take in the socket for the session & a reference to the Broker class
    BrokerSession(boost::asio::ip::tcp::socket socket, Broker& broker);
    void run();
    //this could be private
    void sendmsg(const std::string& msg);

private:
    void wait_for_request();
    void do_write();
    void handle_incoming_message(std::string& data);
    std::vector<std::string> deserializemsg(std::string& data, std::vector<std::string>& out_vec);
    


    boost::asio::ip::tcp::socket s_socket;
    boost::asio::streambuf s_buffer;
    std::deque<std::shared_ptr<std::string>> write_queue_;
    Broker& s_broker;
};

#endif