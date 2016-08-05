#ifndef XBEE_H
#define XBEE_H

class Xbee
{
    public:
        Xbee (); 
        int send (string);
        string read ();

        string getDataRX();

    private:
        static const PATH = "/dev/ttyAMA0";
        static sem_t semaphore;
        static pthread_mutex_t mutex;
        int uart = -1;
        string dataRX;

        int openUart ();
        bool initMutex (); 
        bool initSemaphore ();
};

#endif
