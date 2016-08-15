#ifndef COMDISPATCHER_H
#define COMDISPATCHER_H

#include "SerialPort.h"

#include <string>
#include <list>
#include <mutex>
#include <unistd.h>

#define MAX_TTL 10
#define TTL_TIME 100

struct Conversation {
    unsigned int id;
    std::mutex* signal;
    int ttl;
    std::string request;
    std::string response = "";
    std::string responseFormat;
    std::mutex manipulate;

    Conversation ();
    Conversation (const Conversation&);
};

class ComDispatcher
{
    public:
        ComDispatcher ();

        unsigned int sendMessage (std::string, std::string, std::mutex*);
        bool removeMessage (unsigned int);
        std::string getResponse (unsigned int);

    private:
        static SerialPort port;
        std::list<Conversation> conversations;
        unsigned int conversationId = 0;

        bool send (std::string);
        bool saveResponse (std::string);
        unsigned int getNewId();
        Conversation* getMessage (unsigned int);

        // Methodes which will be threads.
        void dispatchSendings ();
        void dispatchReceipts ();
        void expireConversations ();
};

#endif
