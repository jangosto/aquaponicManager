#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <string>
#include <iostream>
#include <unistd.h>
#include <vector>

#include <mysql++/mysql++.h>

#define PERIOD_INDEX 0
#define ADDRESS_INDEX 1
#define REQUEST_INDEX 2

class DBManager
{
    public:
        DBManager ();
        DBManager (std::string, std::string, std::string, std::string);

        bool insertWaterTemperature(std::string, std::string, int);
        bool insertAirTemperature(std::string, std::string, int);
        bool insertWaterPH(std::string, std::string, int);
        mysqlpp::StoreQueryResult getMonitoringRequests();

    private:
        std::string databaseName;
        std::string databaseHost;
        std::string databaseUser;
        std::string databasePass;

        mysqlpp::StoreQueryResult getControllerByAddress(std::string);
        mysqlpp::StoreQueryResult getDataTypeByName(std::string);
        bool insertData(std::string, std::string, std::string, int);
};

#endif
