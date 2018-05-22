#include <ArduinoJson.h>

#include "WiFiEsp.h"

char ssid[] = "";       // your network SSID (name)
char pass[] = "";    // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char server[] = "techthulhu.com";
int port = 80;
char method[] = "/module/status/json";

unsigned long lastConnectionTime = 0;         // last time connected to the server, in milliseconds
const unsigned long postingInterval = 10000L; // delay between updates, in milliseconds

char lastFaction[1] = "X"; //One character for checking the previous portal state
int lastHealth = 100;

char specificUser[] = "YouKnowWho";
int specificUserLength = 10;

// Initialize the WiFi client object
WiFiEspClient client;

void setup()
{
  // initialize serial for debugging
  Serial.begin(115200);
  // initialize serial for ESP module
  Serial3.begin(115200);
  // initialize ESP module
  WiFi.init(&Serial3);

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");

  SetupRelay();
 
  printWifiStatus();
}

void loop()
{
  char headerEnd[] = "\r\n\r\n";
 
  // Will enter this after the http request has been processed and the loop reinitializes
  if (client.available()) {
    if(client.find(headerEnd))
    {

        const size_t bufferSize = JSON_ARRAY_SIZE(8) + 9*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(10) + 780;
        DynamicJsonBuffer jsonBuffer(bufferSize);       
        JsonObject& root = jsonBuffer.parseObject(client);
        if (!root.success()) {
          Serial.println("Parsing failed!");
        }
        else
        {
          Serial.println("Parsed!");
          JsonObject& result = root["result"];
          const char* controllingFaction = result["controllingFaction"];
          Serial.print("Faction: ");
          Serial.println(controllingFaction);
          const char* owner = result["owner"];
          Serial.print("Owner: ");
          Serial.println(owner);
          if(strncmp(specificUser, owner, specificUserLength) == 0)
          {
            Serial.println("User Detected");
            SpecificUser();
          }
         
          if(controllingFaction[0] != lastFaction[0])
          {
            char newFaction[1] = { controllingFaction[0] };
            Serial.print("Faction Changed to ");
            Serial.print(newFaction[0]);
            Serial.print(" from ");
            Serial.println(lastFaction[0]);

            if(newFaction[0] == 'R' || newFaction[0] == 'E')//Single quote for char literal
            {
              Serial.println("Play Capture Sequence");
              lastHealth = 100;
              CaptureSequence(newFaction);
            }

            lastFaction[0] = newFaction[0];
           
          }
          else
          {
            int health = result["health"];
            Serial.print("Health:");
            Serial.println(result["health"].as<char*>());
            if(health < lastHealth)
            {
               Serial.println("Portal Health decreased!");
               AttackSequence();
               lastHealth = health;
            }
          }
         

        }
    }
  }

  // if 10 seconds have passed since last connection,
  // then connect again and request portal info
  if (millis() - lastConnectionTime > postingInterval) {
    Serial.println("10s has elapsed since last request");
    httpRequest();
  }
}

// this method makes a HTTP connection to the server
void httpRequest()
{
  Serial.println();
   
  // close any connection before send a new request
  // this will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection
  if (client.connect(server, port)) {
    Serial.println("Connecting...");
   
    // send the HTTP GET request
    client.print("GET ");
    client.print(method);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made
    lastConnectionTime = millis();
  }
  else {
    // if you couldn't make a connection
    Serial.println("Connection failed");
  }
}

void SetupRelay() {
  for (int i = 3; i <= 10; i++) {
    pinMode(i, INPUT_PULLUP);
    pinMode(i, OUTPUT); // defaults HIGH (relays off)
  }
  Serial.println("Relay Setup");
  digitalWrite(10, LOW);
  digitalWrite(5, LOW);
  Serial.println("Frame On, Door On!");
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


void SpecificUser()
{
    digitalWrite(10, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(8, LOW);
    delay(500);

    digitalWrite(4, LOW);
    digitalWrite(3, LOW);
    delay(1000);
    digitalWrite(4, HIGH);
    digitalWrite(3, HIGH);  
    delay(1000);
    digitalWrite(4, LOW);
    digitalWrite(3, LOW);
    delay(1000);
    digitalWrite(4, HIGH);
    digitalWrite(3, HIGH); 
    delay(1000);
    digitalWrite(4, LOW);
    digitalWrite(3, LOW);
    delay(1000);
    digitalWrite(4, HIGH);
    digitalWrite(3, HIGH);
    delay(1000);
    digitalWrite(4, LOW);
    digitalWrite(3, LOW);
    delay(1000);
    digitalWrite(4, HIGH);
    digitalWrite(3, HIGH);           
    
    digitalWrite(8, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(10, LOW);    
}

void AttackSequence()
{
      Serial.println("Play Attack Sequence!");
      digitalWrite(10, HIGH);
      digitalWrite(5, HIGH);
      delay(200);
      digitalWrite(10, LOW);
      digitalWrite(5, LOW);
      delay(200);
      digitalWrite(10, HIGH);
      digitalWrite(5, HIGH);
      delay(200);
      digitalWrite(10, LOW);
      digitalWrite(5, LOW);
      delay(200);
      digitalWrite(10, HIGH);
      digitalWrite(5, HIGH);
      delay(200);
      digitalWrite(10, LOW);
      digitalWrite(5, LOW);
      delay(200);
      digitalWrite(10, HIGH);
      digitalWrite(5, HIGH);
      delay(200);
      digitalWrite(10, LOW);
      digitalWrite(5, LOW);
      delay(200);
      digitalWrite(10, HIGH);
      digitalWrite(5, HIGH);
      delay(200);
      digitalWrite(10, LOW);
      digitalWrite(5, LOW);                      
      Serial.println("Attack Loop Complete!");               
}

void CaptureSequence(char faction[1])
{
    Serial.print("Capture Sequence for: ");
    Serial.println(faction[0]);
    int factionPin = 3;
    if(faction[0] == 'R') { factionPin = 4;}
 
    //10 = Frame, 9 = Doors Open, 8 = Lobster Body, 7 = Lobster Wide, 6 = Lobster Closed, 5 = Doors Closed, 4 = Resistance, 3 = Enlightened
    //Frame On
    digitalWrite(10, LOW);
    digitalWrite(5, LOW);
    delay(3000);
    digitalWrite(5, HIGH);
    digitalWrite(9, LOW);
    delay(500);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
    digitalWrite(factionPin, LOW);
    delay(1000);
    digitalWrite(7, HIGH);
    digitalWrite(6, LOW);
    delay(200);
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    delay(200);
    digitalWrite(7, HIGH);
    digitalWrite(6, LOW);
    delay(200);
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    delay(200);
    digitalWrite(7, HIGH);
    digitalWrite(6, LOW);
    delay(200);
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    delay(200);
    digitalWrite(7, HIGH);
    digitalWrite(6, LOW);
    delay(200);
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    delay(200);
    digitalWrite(7, HIGH);
    digitalWrite(6, LOW);      
    delay(200);
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(factionPin, HIGH);
    delay(200);
    digitalWrite(7, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(factionPin, LOW);
    delay(200);
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(factionPin, HIGH);
    delay(200);
    digitalWrite(7, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(factionPin, LOW);
    delay(200);                     
    digitalWrite(factionPin, HIGH);
    delay(200);
    digitalWrite(factionPin, LOW);
    delay(200);                     
    digitalWrite(factionPin, HIGH);
    delay(200);
    digitalWrite(factionPin, LOW);
    delay(200);                     
    digitalWrite(factionPin, HIGH);
    delay(200);
    digitalWrite(factionPin, LOW);
    delay(2000);                     
    digitalWrite(factionPin, HIGH);  
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);      
    digitalWrite(9, HIGH);
    //Doors Close!
    digitalWrite(5, LOW);
    delay(2000);
}

