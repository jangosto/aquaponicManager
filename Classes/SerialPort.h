#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <stdio.h>
#include <string>

#include <unistd.h>     //Used for UART
#include <fcntl.h>      //Used for UART
#include <termios.h>    //Used for UART

#include <mutex>
#include <semaphore.h>

#define SERIAL_PATH "/dev/ttyAMA0"

class SerialPort
{
    public:
        SerialPort (); 
        int send (std::string);
        int read ();

        std::string getDataRX ();
        int getUart();

    private:
        static std::mutex rxMutex;
        static std::mutex txMutex;
        int uart = -1;
        std::string dataRX;

        int openUart ();
};

#endif
