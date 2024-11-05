#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H
#include <iostream>

class ChatMessage
{
    private:
        std::string  m_sender;
        std::string m_body; 
        int m_timestamp;
    
    public:
        ChatMessage(std::string sender, std::string body, int timestamp);
        
        std::string getSender();
        void setSender(std::string sender);

        std::string getBody();
        void setBody(std::string body);

        int getTimestamp();
        void setTimestamp(int timestamp);
    

};

#endif