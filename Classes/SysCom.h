#ifndef SYSCOM_H
#define SYSCOM_H

class SysCom
{
    public:
        SysCom ();
        SysCom (string);

        string getAddress();
        bool getEnvironmentalTemperature ();
        bool getWaterTemperature ();
        bool getDataReady ();

    private:
        static Xbee connection;
        string command;
        string address;
        float value;
        bool dataReady;

        bool init();
        bool sendMessage();
        bool processResponse(string);
};

#endif
