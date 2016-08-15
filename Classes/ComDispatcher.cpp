#include "ComDispatcher.h"

SerialPort ComDispatcher::port;

Conversation::Conversation()
{
}

Conversation::Conversation(const Conversation&)
{
}

ComDispatcher::ComDispatcher()
{
}

unsigned int ComDispatcher::sendMessage(std::string message, std::string responseFormat, std::mutex* signal)
{
    Conversation element;

    element.ttl = 0;
    element.signal = signal;
    element.request = message;
    element.responseFormat = responseFormat;
    element.id = getNewId();
    conversations.push_back(element);

    return element.id;
}

void ComDispatcher::dispatchSendings()
{
    std::list<Conversation>::iterator current;

    while (1) {
        for (current = conversations.begin(); current != conversations.end(); ++current) {
            if (current->ttl == 0 && (current->response).length() == 0) {
                send(current->request);
                current->ttl = MAX_TTL;
            }
        }
    }
}

bool ComDispatcher::send (std::string message)
{
    bool response = true;
    int serialPortResponse = 0;

    serialPortResponse = port.send(message);
    if (serialPortResponse == 0 || serialPortResponse == 2) {
        response = false;
    }

    return response;
}

void ComDispatcher::dispatchReceipts()
{
    while (1) {
        if (port.read() > 0) {
            saveResponse(port.getDataRX());
        }
    }
}

bool ComDispatcher::saveResponse(std::string response)
{
    std::list<Conversation>::iterator current;

    for (current = conversations.begin(); current != conversations.end(); ++current) {
        if (response.find(current->responseFormat) != std::string::npos) {
            current->manipulate.lock();
            current->response = response;
            (current->signal)->unlock();
            current->manipulate.unlock();
            return true;
        }
    }

    return false;
}

unsigned int ComDispatcher::getNewId()
{
    conversationId++;

    return conversationId;
}

bool ComDispatcher::removeMessage(unsigned int id)
{
    std::list<Conversation>::iterator current;

    for (current = conversations.begin(); current != conversations.end(); ++current) {
        if (current->id == id) {
            conversations.erase(current);
            return true;
        }
    }

    return false;
}

std::string ComDispatcher::getResponse(unsigned int id)
{
    Conversation* message;

    message = getMessage(id);

    return message->response;
}

Conversation* ComDispatcher::getMessage(unsigned int id)
{
    std::list<Conversation>::iterator current;

    for (current = conversations.begin(); current != conversations.end(); ++current) {
        if (current->id == id) {
            return &(*current);
        }
    }

    return NULL;
}

void ComDispatcher::expireConversations()
{
    std::list<Conversation>::iterator current;

    while (1) {
        for (current = conversations.begin(); current != conversations.end(); ++current) {
            if (current->ttl > 0 && (current->response).length() == 0) {
                (current->ttl)--;
            }
        }
        usleep(TTL_TIME);
    }
}
