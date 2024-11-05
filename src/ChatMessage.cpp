#include <iostream>
#include "ChatMessage.h" // Ensure the header file name matches

// Constructor definition
ChatMessage::ChatMessage(std::string sender, std::string body, int timestamp)
    : m_sender(sender), 
      m_body(body), 
      m_timestamp(timestamp) {
    // Constructor body (optional)
}

// Getter for m_sender
std::string ChatMessage::getSender() {
    return m_sender;
}

// Setter for m_sender
void ChatMessage::setSender(std::string sender) {
    m_sender = sender;
}

// Getter for m_body
std::string ChatMessage::getBody() {
    return m_body;
}

// Setter for m_body
void ChatMessage::setBody(std::string body) {
    m_body = body;
}

// Getter for m_timestamp
int ChatMessage::getTimestamp() {
    return m_timestamp;
}

// Setter for m_timestamp
void ChatMessage::setTimestamp(int timestamp) {
    m_timestamp = timestamp;
}
