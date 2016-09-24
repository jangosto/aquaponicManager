#include "Controller.h"

ComDispatcher Controller::dispatcher;

Controller::Controller()
{
}

Controller::Controller(std::string addr)
{
    address = addr;
    dispatcher.activate();
}

std::string Controller::getDesiredData()
{
    unsigned int messageId;
    std::string message;
    std::mutex signal;
    std::string response;
    std::string result = "";
    std::string responseFormat = "";
    bool validResponse = false;

    signal.lock();
    message = createMessage();
    messageId = dispatcher.sendMessage(message + "*", message, &signal);

    while (validResponse != true) {
        signal.lock();

        printf("\n[INFO][Controller::getDesiredData] Cheking Response...");

        response = dispatcher.getResponse(messageId);
        
        if (response[0] == '*' && response[1] == '*' && response.back() == '*' ) {
            validResponse = true;
        } else {
            validResponse = false;
            dispatcher.removeResponseFromMessage (messageId);
        }
    }
    dispatcher.removeMessage (messageId);

    cleanResponse(&response);

    printf("\n[INFO][Controller::getDesiredData] Valid response %s", response.c_str());

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

bool Controller::cleanResponse(std::string* response)
{
    bool result = false;


    response->erase(response->length() - 1, 1);
    response->erase(0, 2);

    if (response->front() != '*' && response->back() != '*') {
        result = true;
    }

    return result;
}

bool Controller::processResponse(std::string message)
{
    std::vector<std::string> dataArray;

    dataArray = Tools::StrTools::split(message, '_');

    if (dataArray.size() >= 3) {
        value.assign(dataArray[2].c_str());
    }

    return true;
}

std::string Controller::getAddress()
{
    return address;
}
