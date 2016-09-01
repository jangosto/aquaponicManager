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
    return insertData(std::string dataType, std::string address, std::string value, int timestamp);
}

bool DBManager::insertWaterPH(std::string address, std::string value, int timestamp)
{
    std::string dataType = "Water pH";
    return insertData(std::string dataType, std::string address, std::string value, int timestamp);
}

bool DBManager::insertData(std::string dataType, std::string address, std::string value, int timestamp)
{
    try {
        StoreQueryResult results = getControllerByAddress(address);
        int controllerId = results[0]["id"];

        results = getDataTypeByName(dataType);
        int dataTypeId = results[0]["id"];

        mysqlpp::Connection conn(false);
        conn.connect(dbName.c_str(), dbHost.c_str(), dbUser.c_str(), dbPass.c_str());

        Query query = conn.query();
        query << "INSERT INTO data" <<
                    "(controller_id, datatype_id, time, value) " <<
                    "VALUES ('" << controllerId << "', '" << dataTypeId << "', " << value.c_str() << ", " << timestamp << ");";
        query.execute();
    } catch (BadQuery er) {
        return false;
    } catch (const BadConversion& er) {
        return false;
    } catch (const Exception& er) {
        return false;
    }

    return true;
}

StoreQueryResult DBManager::getControllerByAddress(str::string address)
{
    StoreQueryResult results = NULL;
    
    try {
        mysqlpp::Connection conn(false);
        conn.connect(dbName.c_str(), dbHost.c_str(), dbUser.c_str(), dbPass.c_str());

        Query query = conn.query();
        query << "SELECT * FROM controller" <<
                    "WHERE address='" << address.c_str() << "';";
        results = query.store();
    } catch (BadQuery er) {
        return NULL;
    } catch (const BadConversion& er) {
        return NULL;
    } catch (const Exception& er) {
        return NULL;
    }

    return results;
}

StoreQueryResult DBManager::getDataTypeByName(std::string dataTypeName)
{
    StoreQueryResult results = NULL;

    try {
        mysqlpp::Connection conn(false);
        conn.connect(dbName.c_str(), dbHost.c_str(), dbUser.c_str(), dbPass.c_str());

        Query query = conn.query();
        query << "SELECT * FROM datatype" <<
                    "WHERE name='" << dataTypeName.c_str() << "';";
        results = query.store();
    } catch (BadQuery er) {
        return NULL;
    } catch (const BadConversion& er) {
        return NULL;
    } catch (const Exception& er) {
        return NULL;
    }

    return results;
}
