#include <RFM69.h>
#include <SPI.h>

//#define FREQUENCY   RF69_433MHZ
#define FREQUENCY     RF69_915MHZ

#define CS_PIN  10
#define INT_PIN 40
#define RST_PIN 41

// Addresses for this node. CHANGE THESE FOR EACH NODE!
#define NETWORKID     0   // Must be the same for all nodes
#define MYNODEID      1   // My node ID
#define TONODEID      2   // Destination node ID

#define ENCRYPT       true // Set to "true" to use encryption
#define ENCRYPTKEY    "TOPSECRETPASSWRD" // Use the same 16-byte key on all nodes

#define USEACK        true // Request ACKs or not

// Create radio object
RFM69 radio(CS_PIN, INT_PIN, true, &SPI);

void setup()
{
  Serial.begin(9600);

  pinMode(RST_PIN, OUTPUT);
  digitalWrite(RST_PIN, LOW);
  
  // manual reset
  digitalWrite(RST_PIN, HIGH);
  delay(10);
  digitalWrite(RST_PIN, LOW);
  delay(10);

  // Initialize the RFM69HCW:
  radio.initialize(FREQUENCY, MYNODEID, NETWORKID);
  radio.setHighPower(); // Always use this for RFM69HCW
  
  if (ENCRYPT)
    radio.encrypt(ENCRYPTKEY);

  Serial.print("Node ");
  Serial.print(MYNODEID,DEC);
  Serial.println(" successfully initialized!");  
}

void loop()
{
  // Set up a "buffer" for characters that we'll send:
  static char sendbuffer[62];
  static int sendlength = 0;

  // SENDING
  if (Serial.available() > 0)
  {
    char input = Serial.read();

    if (input != '\r' && input != '\n') // not a carriage return
    {
      sendbuffer[sendlength] = input;
      sendlength++;
    }

    // If the input is a carriage return, or the buffer is full:

    if ((input == '\r') || (input == '\n') || (sendlength == 61)) // CR or buffer full
    {
      // Send the packet!
      Serial.print("sending to node ");
      Serial.print(TONODEID, DEC);
      Serial.print(", message [");
      for (byte i = 0; i < sendlength; i++)
        Serial.print(sendbuffer[i]);
      Serial.println("]");

      // There are two ways to send packets. If you want acknowledgements, use sendWithRetry():
      if (USEACK)
      {
        if (radio.sendWithRetry(TONODEID, sendbuffer, sendlength))
          Serial.println("ACK received!");
        else
          Serial.println("no ACK received");
      }

      // If you don't need acknowledgements, just use send():
      else // don't use ACK
      {
        radio.send(TONODEID, sendbuffer, sendlength);
      }

      sendlength = 0; // reset the packet
    }
  }

  // RECEIVING
  if (radio.receiveDone()) // Got one!
  {
    // Print out the information:

    Serial.print("received from node ");
    Serial.print(radio.SENDERID, DEC);
    Serial.print(", message [");

    // The actual message is contained in the DATA array,
    // and is DATALEN bytes in size:

    for (byte i = 0; i < radio.DATALEN; i++)
      Serial.print((char)radio.DATA[i]);

    // RSSI is the "Receive Signal Strength Indicator", smaller numbers mean higher power.

    Serial.print("], RSSI ");
    Serial.println(radio.RSSI);

    // Send an ACK if requested.
    if (radio.ACKRequested())
    {
      radio.sendACK();
      Serial.println("ACK sent");
    }
  }
}
