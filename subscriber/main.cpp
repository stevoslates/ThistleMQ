#include <iostream>
#include <boost/asio.hpp>
#include "subscriber.h"  

int main() {
    try {
        boost::asio::io_context io_context;

        // Create shared_ptr of subscriber (IMPORTANT because we use shared_from_this)
        auto subscriber = std::make_shared<Subscriber>(io_context, "sports");

        subscriber->start();

        io_context.run();

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
