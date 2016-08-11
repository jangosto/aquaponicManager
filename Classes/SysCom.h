#ifndef SYSCOM_H
#define SYSCOM_H

#include "Tools.h"
#include "SerialPort.h"

#include <stdio.h>
#include <string>
#include <stdbool.h>
#include <vector>
#include <cmath>
#include <limits>

class SysCom
{
    public:
        SysCom ();
        SysCom (std::string);

        std::string getAddress();
        bool getEnvironmentalTemperature ();
        bool getWaterTemperature ();
        bool getDataReady ();
        SerialPort getPort ();

    private:
        static SerialPort port;
        std::string command;
        std::string address;
        float value = std::numeric_limits<float>::quiet_NaN();
        bool dataReady;

        bool init();
        bool sendMessage();
        bool processResponse(std::string);
};

#endif
