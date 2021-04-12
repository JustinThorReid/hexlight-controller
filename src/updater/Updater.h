#ifndef _c_updater
#define _c_updater

#include "../../secrets.h"
#include <WiFi.h>
#include <InternalStorageESP.h>
#include <ArduinoHttpClient.h>

class Updater
{
public:
    void loop();

    static Updater *getInstance()
    {
        static Updater instance;
        return &instance;
    };

private:
    Updater();
};

#endif
