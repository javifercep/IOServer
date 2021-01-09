/*
  DHCP Chat  Server

  A simple server that distributes any incoming messages to all
  connected clients.  To use telnet to  your device's IP address and type.
  You can see the client's input in the serial monitor as well.
  Using an Arduino Wiznet Ethernet shield.

  THis version attempts to get an IP address using DHCP

  Circuit:
   Ethernet shield attached to pins 10, 11, 12, 13

  created 21 May 2011
  modified 9 Apr 2012
  by Tom Igoe
  Based on ChatServer example by David A. Mellis

*/

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
};
IPAddress ip(192, 168, 0, 177);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);

// telnet defaults to port 23
EthernetServer server(5001);
boolean gotAMessage = false; // whether or not you got a message from the client yet

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  // start the Ethernet connection:
  Serial.println("Trying to get an IP address using DHCP");
  //  if (Ethernet.begin(mac) == 0) {
  //    Serial.println("Failed to configure Ethernet using DHCP");
  //  }

  // initialize the ethernet device not using DHCP:
  Ethernet.begin(mac, ip, gateway, subnet);
  // print your local IP address:
  Serial.print("My IP address: ");
  ip = Ethernet.localIP();
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(ip[thisByte], DEC);
    Serial.print(".");
  }
  for (int i = 0; i < 14; i++)
  {
    pinMode(i, OUTPUT);
  }
  Serial.println();
  // start listening for clients
  server.begin();

  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

}

void loop() {
  // wait for a new client:
  EthernetClient client = server.available();

  // when the client sends the first byte, say hello:
  if (client) {

    if (client.available())
    {
      String reply = "n000\n";
      // read the bytes incoming from the client:
      char thisChar = client.read();

      // add it to the inputString:
      inputString += thisChar;

      if (thisChar == '\n') {
        Serial.print(inputString);
        stringComplete = true;
      }

      // print the string when a newline arrives:
      if (stringComplete) {
        switch (inputString.charAt(0))
        {
          case 'w':
            {
              reply = inputString;
              int pin = reply.substring(1, 3).toInt();
              int value = reply.substring(3, 4).toInt();
              digitalWrite(pin, value);
              reply.replace('w', 'a');
            }
            break;
          case 'r':
            {
              reply = inputString;
              int pin = reply.substring(1, 3).toInt();
              int value = digitalRead(pin);
              reply.replace('r', 'a');
              reply.setCharAt(3, value + '0');
            }
            break;
          default:
            /* Nothing */
            break;
        }

        inputString = "";
        stringComplete = false;
        char res[5];
        reply.toCharArray(res, 5);
        // echo the bytes back to the client:
        client.write(res);
        // echo the bytes to the server as well:
        Serial.print(reply);
      }
    }
  }

  if (Serial.available()) {
    while (Serial.available())
    {
      Serial.read();
    }

    // print your local IP address:
    Serial.print("My IP address: ");
    ip = Ethernet.localIP();
    for (byte thisByte = 0; thisByte < 4; thisByte++) {
      // print the value of each byte of the IP address:
      Serial.print(ip[thisByte], DEC);
      Serial.print(".");
    }
  }
}
