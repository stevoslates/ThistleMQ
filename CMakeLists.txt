cmake_minimum_required(VERSION 3.20)
project(MessagingBrokerProject)

# Use C++17
set(CMAKE_CXX_STANDARD 17)

# Add Boost if needed (you may have this handled by package manager)
find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})

# Include directories (so all targets see each other's headers)
include_directories(
    ${PROJECT_SOURCE_DIR}/protocol
    ${PROJECT_SOURCE_DIR}/broker
    ${PROJECT_SOURCE_DIR}/publisher
    ${PROJECT_SOURCE_DIR}/subscriber
)

# ---------------- BROKER ----------------
add_executable(broker
    broker/main.cpp
    broker/Broker.cpp
    broker/BrokerSession.cpp
)

target_link_libraries(broker
    ${Boost_LIBRARIES}
)

# ---------------- PUBLISHER ----------------
add_executable(publisher
    publisher/main.cpp
    publisher/Publisher.cpp
)

target_link_libraries(publisher
    ${Boost_LIBRARIES}
)

# ---------------- SUBSCRIBER ----------------
add_executable(subscriber
    subscriber/main.cpp
    subscriber/Subscriber.cpp
)

target_link_libraries(subscriber
    ${Boost_LIBRARIES}
)
