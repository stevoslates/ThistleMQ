# ThistleMQ - Lightweight Pub/Sub Messaage Broker in C++

## Overview
ThistleMQ is a C++ implemented message broker, currently allowing:

- Multiple publishers to send messages to specific topics.

- Multiple subscribers to receive real-time updates for topics they subscribe to.

- The broker manages all topic state and routes published messages to the correct subscribers.

- Also contains a simple messaging protocol

Note: This is not production-ready code. The current implementation serves as a first fully working version focused on core stream handling and message routing.
Many additional features, improvements, and robustness still need to be added.
This project is primarily for educational and learning purposes at this stage.

## Technologies Used

- C++17
- Boost.Asio (thank god for [this example](https://www.codingwiththomas.com/blog/boost-asio-server-client-example)).
    - Used Boost for building TCP socket management
    - Async read/writes
    - Non-blocking connection handling

## To-Do

- Build out CLI (doesnt have yet, still hardcoded everything into the respective ```main.cpp```)
- Dynamic Subscription (currently one subscriber, one topic, known at build)
- Improved protocol (currently too much copying of strings when could be made more efficient)
- Broker to Subscriber message ACKs when succesful subscription
- Loads of error handling