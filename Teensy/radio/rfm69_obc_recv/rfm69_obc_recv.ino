#include <RFM69.h>
#include <SPI.h>

#define FREQUENCY   RF69_433MHZ
//#define FREQUENCY     RF69_915MHZ

#define CS_PIN  38
#define INT_PIN 8
#define RST_PIN 7

#define SPI_MISO 39
#define SPI_MOSI 26
#define SPI_SCK 27

// Addresses for this node. CHANGE THESE FOR EACH NODE!
#define NETWORKID     0   // Must be the same for all nodes
#define MYNODEID      2   // My node ID
#define TONODEID      1   // Destination node ID

#define ENCRYPT       true // Set to "true" to use encryption
#define ENCRYPTKEY    "TOPSECRETPASSWRD" // Use the same 16-byte key on all nodes

#define USEACK        true // Request ACKs or not

// Create radio object
RFM69 radio(CS_PIN, INT_PIN, true, &SPI1);

void setup()
{
  Serial.begin(115200);

  SPI1.setMISO(SPI_MISO);
  SPI1.setMOSI(SPI_MOSI);
  SPI1.setSCK(SPI_SCK);
  //SPI1.setCS(CS_PIN);

  pinMode(RST_PIN, OUTPUT);
  digitalWrite(RST_PIN, LOW);
  
  // manual reset
  digitalWrite(RST_PIN, HIGH);
  delay(10);
  digitalWrite(RST_PIN, LOW);
  delay(10);

  // Initialize the RFM69HCW:
  while(radio.initialize(FREQUENCY, MYNODEID, NETWORKID) == false) {
    Serial.println("INIT FAILED");
  }
  
  radio.setHighPower(); // Always use this for RFM69HCW
  
  if (ENCRYPT)
    radio.encrypt(ENCRYPTKEY);

  Serial.print("Node ");
  Serial.print(MYNODEID,DEC);
  Serial.println(" successfully initialized!");  
}

void loop()
{
  // RECEIVING
  //radio.receiveBegin();
  
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
  } else {
    //Serial.println("NOTHING RECEIVED");
  }
}
