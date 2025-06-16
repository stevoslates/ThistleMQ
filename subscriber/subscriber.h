#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <string>
#include <boost/asio.hpp>

struct SubConn {
    boost::asio::ip::tcp::socket socket;
    //might need to store something else here
    SubConn(boost::asio::io_context& io_context)
      : socket(io_context) {}
};

class Subscriber : public std::enable_shared_from_this<Subscriber> {

public:
    Subscriber(boost::asio::io_context& io_context, const std::string& topic);
    void start();
    ~Subscriber();
private:
    bool connect(); 
    //FOR NOW: One subscriber per topic, known at build time (will change to allow dynamic subs)
    void subscribe(const std::string& topic); 
    void sendmsg(const std::string& msg);
    void listen();

    SubConn sconn_;
    int sid_;
    static int next_id_;
    const std::string topic_;
    boost::asio::streambuf read_buffer_;
};



#endif