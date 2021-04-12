/**
 * Checks for the existance of a .bin with version APP_VERSION+1 then downloads and restarts with that version.
 * 
 * Will download all versions until it reaches the max version.
 * Increase APP_VERSION macro with each release
 */

#include "Updater.h"

uint8_t APP_VERSION = 1;
WiFiClient wifiClient; // HTTP

Updater::Updater()
{
}

void Updater::loop()
{
    const char *PATH = "%s/v%d.bin";           // Set the URI to the .bin firmware
    const unsigned long CHECK_INTERVAL = 6000; // Time interval between update checks (ms)

    // Time interval check
    static unsigned long previousMillis;
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis < CHECK_INTERVAL)
        return;
    previousMillis = currentMillis;

    // Create the download URL
    char buff[32];
    snprintf(buff, sizeof(buff), PATH, UPDATER_APPNAME, APP_VERSION + 1);

    Serial.print("Check for update file ");
    Serial.println(buff);

    // Make the GET request
    HttpClient client(wifiClient, UPDATER_HOST, UPDATER_PORT);
    client.get(buff);

    int statusCode = client.responseStatusCode();
    Serial.print("Update status code: ");
    Serial.println(statusCode);

    if (statusCode != 200)
    {
        client.stop();
        return;
    }

    long length = client.contentLength();
    if (length == HttpClient::kNoContentLengthHeader)
    {
        client.stop();
        Serial.println("Server didn't provide Content-length header. Can't continue with update.");
        return;
    }
    Serial.print("Server returned update file of size ");
    Serial.print(length);
    Serial.println(" bytes");

    if (!InternalStorage.open(length))
    {
        client.stop();
        Serial.println("There is not enough space to store the update. Can't continue with update.");
        return;
    }

    byte b;
    while (length > 0)
    {
        if (!client.readBytes(&b, 1)) // reading a byte with timeout
            break;
        InternalStorage.write(b);
        length--;
    }
    InternalStorage.close();
    client.stop();

    if (length > 0)
    {
        Serial.print("Timeout downloading update file at ");
        Serial.print(length);
        Serial.println(" bytes. Can't continue with update.");
        return;
    }

    Serial.println("Sketch update apply and reset.");
    Serial.flush();
    InternalStorage.apply(); // this doesn't return
}