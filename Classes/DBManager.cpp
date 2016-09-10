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
        mysqlpp::StoreQueryResult results = getControllerByAddress(address);
        int controllerId = results[0]["id"];

        results = getDataTypeByName(dataType);
        int dataTypeId = results[0]["id"];

        mysqlpp::Connection conn(false);
        conn.connect(databaseName.c_str(), databaseHost.c_str(), databaseUser.c_str(), databasePass.c_str());

        mysqlpp::Query query = conn.query();
        query << "INSERT INTO data" <<
                    "(controller_id, datatype_id, time, value) " <<
                    "VALUES ('" << controllerId << "', '" << dataTypeId << "', " << value.c_str() << ", " << timestamp << ");";
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

        mysqlpp::Query query = conn.query();
        query << "SELECT * FROM controller" <<
                    "WHERE address='" << address.c_str() << "';";
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

        mysqlpp::Query query = conn.query();
        query << "SELECT * FROM datatype" <<
                    "WHERE name='" << dataTypeName.c_str() << "';";
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
