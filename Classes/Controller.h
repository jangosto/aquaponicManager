#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Tools.h"
#include "ComDispatcher.h"

#include <stdio.h>
#include <string>
#include <mutex>

#include <stdbool.h>
#include <vector>
#include <cmath>
#include <limits>

class Controller
{
    public:
        Controller ();
        Controller (std::string);

        std::string getAddress();
        std::string getEnvironmentalTemperature ();
        std::string getWaterTemperature ();

    private:
        std::string command;
        std::string address;
        std::string value = "";
        static ComDispatcher dispatcher;

        std::string createMessage ();
        bool processResponse (std::string);
        std::string getDesiredData ();
};

#endif
