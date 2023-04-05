#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>

const char *networkName = "WiFi Rover";
const char *password = "";

ESP8266WebServer server(80);

String parser;

void setup(void)
{

    // To connect to existing network, use the below code

    /*
    WiFi.mode(WIFI_STA);
    WiFi.begin("ssid", "password");
     */
    // For making ESP network by itself, do below:
    WiFi.mode(WIFI_AP); 
    WiFi.softAP(networkName, password);
    // the default IP for the car is 192.168.4.1

    // Serial communications to computer or arduino
    Serial.begin(115200);

    SPIFFS.begin();

    delay(2000);
    Serial.print("%"); // this is ignored by the arduino, so it should not affect function.
    Serial.println(WiFi.softAPIP());
    //reserve string size; this will reduce the amount of crashing after a long run.
    // read more about stack fragmentation or etc.
    parser.reserve(128);

    // Wait for connection

    // -------------------
    server.on("/data", HTTP_POST, []() {
        if (!(server.hasArg("left") && server.hasArg("right"))) {
            server.send(500, "text/html", "bad, you must have both left and right params");
            return;
        }
        parser = server.arg("left") + ":" + server.arg("right") + "\n";
        server.send(200, "text/html", "OK");
    });

    //otherwise, if we can't find anything ( in our mountpoints) we then call the fileRead function
    server.onNotFound([]() {
        if (!fileRead(server.uri()))
        {
            server.send(404, "text/html",
                        "<h1>404 File not found on SPIFFS</h1>");
        }
    });
    
    MDNS.begin("rover");
    
    server.begin();
}

void loop(void)
{
    server.handleClient();
    MDNS.update();
    
    //this is the super important part, we should send the string to the arduino each time
    Serial.print(parser);
    parser = "";
    Serial.flush();
    delay(100);
}

//read file and stream it over the server connection if the file exists
bool fileRead(String filepath)
{
    if (filepath.endsWith("/"))
        filepath += "index.html";

    if (SPIFFS.exists(filepath))
    {

        File f = SPIFFS.open(filepath, "r");
        server.streamFile(f, contentType(filepath));
        f.close();
        return true;
    }

    return false;
}

//create contentType from the filename
String contentType(String filepath)
{
    if (filepath.endsWith(".html"))
        return "text/html";
    else if (filepath.endsWith(".css"))
        return "text/css";
    else if (filepath.endsWith(".js"))
        return "text/javascript";
    else
        return "text/plain";
}
