#include "SysCom.h"

SerialPort SysCom::port;

SysCom::SysCom()
{
}

SysCom::SysCom(std::string addr)
{
    address = addr;
}

bool SysCom::init()
{
}

bool SysCom::getEnvironmentalTemperature()
{
    command = "AMBTEMP";
    sendMessage();
    processResponse(port.getDataRX());
}

bool SysCom::getWaterTemperature()
{
    command = "WATTEMP";
    sendMessage();
    processResponse(port.getDataRX());
}

bool SysCom::sendMessage ()
{
    bool response = true;
    int serialPortResponse = 0;

    std::string message;

    if (address.length() > 0) {
        message = address;
    } else {
        printf("\n[ERROR][SysCom::sendMessage] Address to send not defined");
        response = false;
    }

    if (command.length() > 0) {
        message = message + "_" + command;
    } else {
        printf("\n[ERROR][SysCom::sendMessage] Command to send not defined");
        response = false;
    }

    if (! isnan(value)) {
         message = message + "_" + std::to_string(value);
         value = std::numeric_limits<float>::quiet_NaN();
    }

    serialPortResponse = port.send(message);
    if (serialPortResponse == 0 || serialPortResponse == 2) {
        response = false;
    }

    return response;
}

bool SysCom::processResponse (std::string message)
{
    std::vector<std::string> dataArray;

    dataArray = Tools::StrTools::split(message, '_');

    if (dataArray.size() >= 3) {
        value = atof(dataArray[2].c_str());
    }

    dataReady = true;

    return true;
}

std::string SysCom::getAddress()
{
    return address;
}

SerialPort SysCom::getPort ()
{
    return port;
}
