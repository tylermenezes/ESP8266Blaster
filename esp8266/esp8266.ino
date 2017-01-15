#include <ESP8266WiFi.h>
#include <IRremoteESP8266.h>

// Configure Wifi:
const char* ssid = "VDevice";
const char* password = "hunter2";

IRsend irsend(4); // Configure IR GPIO (GPIO4 is D2 on NodeMCU)
WiFiServer server(31442); // Configure server port

void setup() {
    Serial.begin(115200);
    delay(10);

    irsend.begin();
  
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    Serial.println(WiFi.localIP()); // You should probably statically assign this...
  
    server.begin();
}

void loop() {
    // Wait for a client to connect
    WiFiClient client = server.available();
    if (!client) {
        return;
    }

    // Wait until the client is ready
    while(!client.available()){
        delay(1);
    }

    // Get the frequency
    int freq = client.read();

    // Get the length of the IR command
    byte dataLengthField[2];
    client.readBytes(dataLengthField, 2);
    int dataLength = (dataLengthField[1] * 256) + dataLengthField[0];

    // Validate length
    if (dataLength % 2 != 0) {
        client.write("\x01");
        client.flush();
        client.stop();
        return;
    }

    // Read the bytes to send
    byte irCommandField[dataLength];
    client.readBytes(irCommandField, dataLength);
    unsigned int irCommand[dataLength/2];
    memset( irCommand, 0, (dataLength/2)*sizeof(unsigned int) );

    for(int i = 0; i < dataLength; i+= 2) {
        irCommand[i/2] = (irCommandField[i+1] * 256) + irCommandField[i];
    }

    // Send the command
    for (int i = 0; i < 10; i++) {
        irsend.sendRaw(irCommand, sizeof(irCommand)/sizeof(unsigned int), freq);
    }

    // Close the connection
    client.write("\00");
    client.flush();
    client.stop();
}
