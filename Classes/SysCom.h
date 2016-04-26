#ifndef SYSCOM_H
#define SYSCOM_H

class SysCom
{
    public:
        SysCom ();
        SysCom (string);

        bool getTemperatures ();

    private:

        string command;
        string address;
        float value;
};

#endif
