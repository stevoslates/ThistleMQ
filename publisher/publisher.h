#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <string>
#include <boost/asio.hpp>

struct PubConn {
    boost::asio::ip::tcp::socket socket;
    
    PubConn(boost::asio::io_context& io_context)
      : socket(io_context) {}
};

class Publisher {

public:
    Publisher(boost::asio::io_context& io_context);
    void sendmsg(const std::string& topic, const std::string& msg);
    ~Publisher(); //should gracefully clean up the connection
private:
    bool connect(); //called upon construction
    
    int pid_;
    static int next_id_;
    PubConn pconn_;
};

#endif