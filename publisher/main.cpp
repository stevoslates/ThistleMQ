#include <iostream>
#include <boost/asio.hpp>
#include "publisher.h" 

int main() {
    try {
        boost::asio::io_context io_context;

        Publisher pub(io_context);

        // For this first simple test, send one message
        pub.sendmsg("sports", "Manchester City win 2-0!");
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));

        std::cout << "Message sent!" << std::endl;

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}