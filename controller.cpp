#include "Classes/Controller.h"
#include "Classes/DBManager.h"

#include <string>
#include <iostream>
#include <thread>
#include <ctime>
#include <vector>

#define DATABASE_NAME "aquaponic"
#define DATABASE_HOST "localhost"
#define DATABASE_USER "root"
#define DATABASE_PASS "regaloneo"

using namespace std;

typedef void (*function_type)(Controller*, int);

void getEnvTemp (Controller* controller, int period);
void getWaterTemp (Controller* controller, int period);
Controller* getControllerIndexInVector (vector<Controller*> list, std::string address);
function_type getFunction(std::string request);

int main (void)
{
    vector<Controller*> controllers;
    vector<std::thread*> threadHandlers;
    vector<std::thread*>::iterator current;
    string controllerAddress;
    string controllerRequest;
    int period;
    Controller* index;
    function_type threadFunctionAddress;

    DBManager* dbconnection = new DBManager(DATABASE_NAME, DATABASE_HOST, DATABASE_USER, DATABASE_PASS);
    mysqlpp::StoreQueryResult config = dbconnection->getMonitoringRequests();

    for (size_t i=0, n=config.num_rows(); i<n; ++i) {
        controllerAddress.assign(config[i]["address"]);
        controllerRequest.assign(config[i]["request"]);
        period = config[i]["period"];
        index = getControllerIndexInVector(controllers, controllerAddress);

        threadFunctionAddress = getFunction(controllerRequest);

        if (index == false) {
            controllers.push_back(new Controller(controllerAddress));

            threadHandlers.push_back(new std::thread (threadFunctionAddress, controllers.back(), period));
        } else {
            threadHandlers.push_back(new std::thread (threadFunctionAddress, index, period));
        }
    }

    for (current = threadHandlers.begin(); current != threadHandlers.end(); ++current) {
        if ((*current)->joinable()) (*current)->join();
    }

    return 5;
}

Controller* getControllerIndexInVector (vector<Controller*> list, std::string address)
{
    vector<Controller*>::iterator current;
    for (current = list.begin(); current != list.end(); ++current) {
        if ((*current)->getAddress() == address) {
            return *current; 
        }
    }

    return false;
}

function_type getFunction(std::string request)
{
    if (request == "Water Temperature") {
        return &getWaterTemp;
    } else if (request == "Air Temperature") {
        return &getEnvTemp;
    } else if (request == "Water pH") {
        return NULL;
    } else {
        return NULL;
    }
}

void getEnvTemp (Controller* controller, int period)
{
    string data = "";
    DBManager* database = new DBManager(DATABASE_NAME, DATABASE_HOST, DATABASE_USER, DATABASE_PASS);

    while (1) {
        data.assign(controller->getEnvironmentalTemperature());
        database->insertAirTemperature(controller->getAddress(), data, static_cast<long int>(time(NULL)));
        sleep(period);
    }
}

void getWaterTemp (Controller* controller, int period)
{
    string data = "";
    DBManager* database = new DBManager(DATABASE_NAME, DATABASE_HOST, DATABASE_USER, DATABASE_PASS);

    while (1) {
        data.assign(controller->getWaterTemperature());
        database->insertWaterTemperature(controller->getAddress(), data, static_cast<long int>(time(NULL)));
        sleep(period);
    }
}
