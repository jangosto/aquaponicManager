#include "SysCom.h"
#include "Tools.h"
#include "Xbee.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <vector>

SysCom::SysCom()
{
    if (connection.uart == -1) {
        init();
    }
}

SysCom::SysCom(string addr)
{
    if (connection.uart == -1) {
        init();
    }
    address = addr;
}

SysCom::init()
{
    connection.openUart();
    connection.initMutex();
    connection.initSemaphore();
}

string SysCom::getEnvironmentalTemperature()
{
    command = "AMBTEMP";
    sendCommand();
    response = connection.getDataRX();
    processResponse(response);
}

string SysCom::getWaterTemperature()
{
    command = "WATTEMP";
    sendMessage();
    response = connection.getDataRX();
    processResponse(response);
}

bool SysCom::sendMessage()
{
    bool response = true;
    int xbeeResponse = 0;

    string message = "";

    if (address.length > 0) {
        message = message + "_" + address;
    } else {
        printf("\n[ERROR][SysCom::sendMessage] Address to send not defined");
        response = false;
    }

    if (command.length > 0) {
        message = message + command;
    } else {
        printf("\n[ERROR][SysCom::sendMessage] Command to send not defined");
        response = false;
    }

    if (value != null) {
         message = message + "_" + (string) value;
         value = null;
    }

    xbeeResponse = connection.send(message);
    if (xbeeResponse == 0 || xbeeResponse == 2) {
        response = false;
    }

    return response;
}

bool SysCom::processResponse(message)
{
    vector<string> dataArray;

    dataArray = explode("_", message);

    if (dataArray.size() >= 3) {
        value = atof(dataArray[2]);
    }

    dataReady = true;

    return true;
}

string getAddress()
{
    return address;
}
