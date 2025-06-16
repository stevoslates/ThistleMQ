#include <boost/asio.hpp>
#include <iostream>
#include "broker.h"



int main() {
    try {
        boost::asio::io_context io_context;

        Broker b(io_context, 8888);

        io_context.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    
    return 0;
}