#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

#define WIFI_SSID           "PU_BIKE"
#define WIFI_PASS           "makerbar"
char * MySQL_db_user          = "Bike_manager";
char * MySQL_db_pass          = "manager";
IPAddress MySQL_server_host_ipAdderss(100, 100, 100, 102);
WiFiClient WIFI_client;
MySQL_Connection MySQL_connector((Client *)&WIFI_client);

void setup()
{
        Serial.begin(115200);
        Serial.println();
        WiFi.begin(WIFI_SSID, WIFI_PASS);
        Serial.print("WiFi Connecting");
        while (WiFi.status() != WL_CONNECTED)
        {
                delay(500);
                Serial.print(".");
        }
        Serial.println();
        Serial.print("Connected, IP address: ");
        Serial.println(WiFi.localIP());
}

void loop()
{
        delay(1000);
        if (MySQL_connector.connected())
        {
                Serial.println("MySQL Connected.");
        }
        else
        {
                MySQL_connector.close();
                Serial.println("MySQL Connecting...");
                if (MySQL_connector.connect(MySQL_server_host_ipAdderss, 7777, MySQL_db_user, MySQL_db_pass))
                {
                        delay(500);
                        Serial.println("MySQL Successful Connect!");
                }
                else
                {
                        Serial.println("MySQL Can't Reconnect!");
                }
        }
}
