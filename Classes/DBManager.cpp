#include "DBManager.h"

DBManager::DBManager()
{
}

DBManager::DBManager(std::string dbName, std::string dbHost, std::string dbUser, std::string dbPass)
{
    databaseName = dbName;
    databaseHost = dbHost;
    databaseUser = dbUser;
    databasePass = dbPass;
}

bool DBManager::insertWaterTemperature(std::string address, std::string value, int timestamp)
{
    std::string dataType = "Water Temperature";
    return insertData(dataType, address, value, timestamp);
}

bool DBManager::insertAirTemperature(std::string address, std::string value, int timestamp)
{
    std::string dataType = "Air Temperature";
    return insertData(dataType, address, value, timestamp);
}

bool DBManager::insertWaterPH(std::string address, std::string value, int timestamp)
{
    std::string dataType = "Water pH";
    return insertData(dataType, address, value, timestamp);
}

bool DBManager::insertData(std::string dataType, std::string address, std::string value, int timestamp)
{
    try {
        std::cout << "\n[INFO][DBManager::insertData]Inserting data in database:";
        mysqlpp::StoreQueryResult results = getControllerByAddress(address);
        int controllerIdInt = results[0]["id"];
        std::string controllerId = std::to_string(controllerIdInt);

        results = getDataTypeByName(dataType);
        int dataTypeIdInt = results[0]["id"];
        std::string dataTypeId = std::to_string(dataTypeIdInt);

        mysqlpp::Connection conn(false);
        conn.connect(databaseName.c_str(), databaseHost.c_str(), databaseUser.c_str(), databasePass.c_str());

        mysqlpp::Query query = conn.query("INSERT INTO data (controller_id, datatype_id, value, time) VALUES ('" + controllerId + "', '" + dataTypeId + "', " + value + ", " + std::to_string(timestamp) + ");");
        query.execute();
    } catch (mysqlpp::BadQuery er) {
        return false;
    } catch (const mysqlpp::BadConversion& er) {
        return false;
    } catch (const mysqlpp::Exception& er) {
        return false;
    }

    return true;
}

mysqlpp::StoreQueryResult DBManager::getControllerByAddress(std::string address)
{
    mysqlpp::StoreQueryResult results;
    
    try {
        mysqlpp::Connection conn(false);
        conn.connect(databaseName.c_str(), databaseHost.c_str(), databaseUser.c_str(), databasePass.c_str());

        mysqlpp::Query query = conn.query("SELECT * FROM controller WHERE address='" + address + "';");
        results = query.store();
    } catch (mysqlpp::BadQuery er) {
        //return NULL;
    } catch (const mysqlpp::BadConversion& er) {
        //return NULL;
    } catch (const mysqlpp::Exception& er) {
        //return NULL;
    }

    return results;
}

mysqlpp::StoreQueryResult DBManager::getDataTypeByName(std::string dataTypeName)
{
    mysqlpp::StoreQueryResult results;

    try {
        mysqlpp::Connection conn(false);
        conn.connect(databaseName.c_str(), databaseHost.c_str(), databaseUser.c_str(), databasePass.c_str());

        mysqlpp::Query query = conn.query("SELECT * FROM datatype WHERE name='" + dataTypeName + "';");
        results = query.store();
    } catch (mysqlpp::BadQuery er) {
        //return NULL;
    } catch (const mysqlpp::BadConversion& er) {
        //return NULL;
    } catch (const mysqlpp::Exception& er) {
        //return NULL;
    }

    return results;
}

mysqlpp::StoreQueryResult DBManager::getMonitoringRequests()
{
    mysqlpp::StoreQueryResult results;

    try {
        mysqlpp::Connection conn(false);
        conn.connect(databaseName.c_str(), databaseHost.c_str(), databaseUser.c_str(), databasePass.c_str());

        mysqlpp::Query query = conn.query("SELECT a.period AS period, b.address AS address, c.name AS request, c.command AS command FROM monitoring_request AS a INNER JOIN controller AS b ON b.id = a.controller_id INNER JOIN datatype as c ON c.id = a.datatype_id;");
        results = query.store();
    } catch (mysqlpp::BadQuery er) {
        //return NULL;
    } catch (const mysqlpp::BadConversion& er) {
        //return NULL;
    } catch (const mysqlpp::Exception& er) {
        //return NULL;
    }

    return results;
}
