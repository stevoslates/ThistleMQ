#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <vector>
#include <sstream>

/*
TO DO: Stop expesnvie copying and creation of these strings, we could try do it in place or use string_views
*/

enum class MessageType {
    PUBLISH,
    SUBSCRIBE,
    MESSAGE,
    UNKNOWN
};

struct BrokerMessage {
    MessageType type;
    std::string topic;
    std::string payload;
};


inline std::string serialize_publish(const std::string& topic, const std::string& payload) {
    return "PUBLISH|" + topic + "|" + payload + '\n';
}

inline std::string serialize_subscribe(const std::string& topic) {
    return "SUBSCRIBE|" + topic + '\n';
}

inline std::string serialize_broker_message(const std::string& topic, const std::string& payload) {
    return "MESSAGE|" + topic + "|" + payload + '\n';
}


inline BrokerMessage deserialize_message(const std::string& data) {
    BrokerMessage msg;
    std::vector<std::string> tokens;

    std::istringstream ss(data);
    std::string segment;
    while (std::getline(ss, segment, '|')) {
        tokens.push_back(segment);
    }

    if (tokens.size() == 0) {
        msg.type = MessageType::UNKNOWN;
        return msg;
    }

    if (tokens[0] == "PUBLISH" && tokens.size() == 3) {
        msg.type = MessageType::PUBLISH;
        msg.topic = tokens[1];
        msg.payload = tokens[2];
    }
    else if (tokens[0] == "SUBSCRIBE" && tokens.size() == 2) {
        msg.type = MessageType::SUBSCRIBE;
        msg.topic = tokens[1];
    }
    else if (tokens[0] == "MESSAGE" && tokens.size() == 3) {
        msg.type = MessageType::MESSAGE;
        msg.topic = tokens[1];
        msg.payload = tokens[2];
    }
    else {
        msg.type = MessageType::UNKNOWN;
    }

    return msg;
}

#endif
