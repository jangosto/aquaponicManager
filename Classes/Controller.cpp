#include "Controller.h"

Controller::Controller()
{
}

Controller::Controller(std::string addr)
{
    address = addr;
}

std::string Controller::getDesiredData()
{
    unsigned int messageId;
    std::string message;
    std::mutex signal;
    std::string response;
    std::string result = "";
    ComDispatcher dispatcher;

    signal.lock();
    message = createMessage();
    messageId = dispatcher.sendMessage(message, message, &signal);
    signal.lock();

    response = dispatcher.getResponse(messageId);

    if (processResponse(response)) {
        result = value;
    }

    return result;
}

std::string Controller::getEnvironmentalTemperature()
{
    command = "AMBTEMP";
    return getDesiredData();
}

std::string Controller::getWaterTemperature()
{
    command = "WATTEMP";
    return getDesiredData();
}

std::string Controller::createMessage()
{
    std::string message;

    if (address.length() > 0) {
        message = address;
    } else {
        printf("\n[ERROR][Controller::sendMessage] Address to send not defined");
        return "";
    }

    if (command.length() > 0) {
        message = message + "_" + command;
    } else {
        printf("\n[ERROR][Controller::sendMessage] Command to send not defined");
        return "";
    }

    if (value.length() > 0) {
         message = message + "_" + value;
         value = "";
    }

    return message;
}

bool Controller::processResponse(std::string message)
{
    std::vector<std::string> dataArray;

    dataArray = Tools::StrTools::split(message, '_');

    if (dataArray.size() >= 3) {
        value = atof(dataArray[2].c_str());
    }

    return true;
}

std::string Controller::getAddress()
{
    return address;
}
