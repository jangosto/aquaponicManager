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
        std::string useMessage (std::string);

    private:
        std::string command;
        std::string address;
        std::string value = "";
        static ComDispatcher dispatcher;
        std::mutex commandMutex;
        std::mutex valuePropMutex;

        std::string createMessage ();
        bool cleanResponse (std::string*);
        bool processResponse (std::string);
        std::string getDesiredData (std::string);
};

#endif
