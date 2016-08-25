#ifndef COMDISPATCHER_H
#define COMDISPATCHER_H

#include "SerialPort.h"

#include <string>
#include <list>
#include <mutex>
#include <unistd.h>
#include <thread>

#define MAX_TTL 10
#define TTL_TIME 100000

struct Conversation {
    unsigned int id;
    std::mutex* signal;
    int ttl;
    std::string request;
    std::string response = "";
    std::string responseFormat;
//    std::mutex manipulate;

    Conversation ();
    Conversation (const Conversation&);
};

class ComDispatcher
{
    public:
        ComDispatcher ();
        ~ComDispatcher ();

        unsigned int sendMessage (std::string, std::string, std::mutex*);
        bool removeMessage (unsigned int);
        bool removeResponseFromMessage (unsigned int);
        std::string getResponse (unsigned int);
        bool activate ();

    private:
        static SerialPort port;
        std::list<Conversation> conversations;
        unsigned int conversationId = 0;
        std::mutex listMx;

        bool send (std::string);
        bool saveResponse (std::string);
        unsigned int getNewId();
        Conversation* getMessage (unsigned int);

        // Threads.
        std::thread* sendThread;
        std::thread* receiveThread;
        std::thread* expireThread;
        bool stop_sendThread = false;
        bool stop_receiveThread = false;
        bool stop_expireThread = false;

        void dispatchSendings ();
        void dispatchReceipts ();
        void expireConversations ();
};

#endif
