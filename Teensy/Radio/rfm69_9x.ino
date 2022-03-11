#include <SPI.h>
#include <RFM69.h>
#include <RH_RF95.h>
#include <RHHardwareSPI1.h>

/* RADIO FREQUENCY CONFIGS */
//#define RFM69_FREQUENCY   RF69_433MHZ
#define RFM69_FREQUENCY     RF69_915MHZ
#define RFM9x_FREQUENCY   434.0

/* RFM69 PIN CONFIG */
#define RFM69_CS_PIN  38
#define RFM69_INT_PIN 8
#define RFM69_RST_PIN 7

/* RFM9x PIN CONFIG */
#define RFM9x_CS_PIN  37
#define RFM9x_INT_PIN 30
#define RFM9x_RST_PIN 38

/* SPI MISO/MOSI/SCK CONFIG */
#define SPI_MISO 39
#define SPI_MOSI 26
#define SPI_SCK 27

/* Addresses for this node. CHANGE THESE FOR EACH NODE! */
#define NETWORKID     0   // Must be the same for all nodes
#define MYNODEID      2   // My node ID
#define TONODEID      1   // Destination node ID

#define ENCRYPT       true // Set to "true" to use encryption
#define ENCRYPTKEY    "TOPSECRETPASSWRD" // Use the same 16-byte key on all nodes

#define USEACK        true // Request ACKs or not

/* Create radio objects */
RFM69 rfm69(RFM69_CS_PIN, RFM69_INT_PIN, false, &SPI1);
RH_RF95 rfm9x(RFM9x_CS_PIN, RFM9x_INT_PIN, hardware_spi1);

/* Function Defs */
void reset_radios();
void init_RFM69();
void init_RFM9x();
void RFM69_send();
void RFM69_recv();
void RFM9x_send();
void RFM9x_recv();

/* Global Variables */
static char sendbuffer[62];
static int sendlength = 0;
int16_t packetnum = 0;
bool radio_cs = false;

/* ---------- SETUP ---------- */
void setup()
{
  Serial.begin(115200);

  SPI1.setMISO(SPI_MISO);
  SPI1.setMOSI(SPI_MOSI);
  SPI1.setSCK(SPI_SCK);

  pinMode(RFM69_RST_PIN, OUTPUT);
  digitalWrite(RFM69_RST_PIN, LOW);
  pinMode(RFM9x_RST_PIN, OUTPUT);
  digitalWrite(RFM9x_RST_PIN, LOW);

  // Manually reset radios
  reset_radios();

  // Initialize the RFM69:
  init_RFM69();
  // Initialize the RFM9x:
  init_RFM9x();
}

/* ---------- MAIN LOOP ---------- */
void loop()
{
  if(radio_cs) {
    RFM69_send();
    RFM69_recv();
  }
  if(!radio_cs) {
    RFM9x_send();
    RFM9x_recv();
  }
}

/* ---------- HARD RESET RADIOS ---------- */
void reset_radios() {
  digitalWrite(RFM69_RST_PIN, HIGH);
  digitalWrite(RFM9x_RST_PIN, LOW);
  delay(10);
  digitalWrite(RFM69_RST_PIN, LOW);
  digitalWrite(RFM9x_RST_PIN, HIGH);
  delay(10);
}

/* ---------- INITIALIZE RFM69 ---------- */
void init_RFM69() {
  while(!(rfm69.initialize(RFM69_FREQUENCY, MYNODEID, NETWORKID))) {
    Serial.println("RFM69 INIT FAILED");
  }
  rfm69.setHighPower(); // Always use this for RFM69HCW
  if (ENCRYPT)
    rfm69.encrypt(ENCRYPTKEY);

  Serial.print("RFM69: Node ");
  Serial.print(MYNODEID,DEC);
  Serial.println(" successfully initialized!");  
}

/* ---------- INITIALIZE RFM9X ---------- */
void init_RFM9x() {
  while (!(rfm9x.init())) {
    Serial.println("RFM9x INIT FAILED");
  }
  rfm9x.setFrequency(RFM9x_FREQUENCY);
  rfm9x.setTxPower(23, false);

  Serial.print("RFM9x: Node ");
  Serial.print(MYNODEID,DEC);
  Serial.println(" successfully initialized!"); 
}

/* ---------- RFM69 SEND ---------- */
void RFM69_send() {

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
      radio_cs = !radio_cs;
      // Send the packet!
      Serial.print("RFM69: sending to node ");
      Serial.print(TONODEID, DEC);
      Serial.print(", message [");
      for (byte i = 0; i < sendlength; i++)
        Serial.print(sendbuffer[i]);
      Serial.println("]");

      // There are two ways to send packets. If you want acknowledgements, use sendWithRetry():
      if (USEACK)
      {
        if (rfm69.sendWithRetry(TONODEID, sendbuffer, sendlength))
          Serial.println("ACK received!");
        else
          Serial.println("no ACK received");
      }

      // If you don't need acknowledgements, just use send():
      else // don't use ACK
      {
        rfm69.send(TONODEID, sendbuffer, sendlength);
      }

      sendlength = 0; // reset the packet
      sendbuffer[0] = '\0';
    }
  }
}

/* ---------- RFM69 RECEIVE ---------- */
void RFM69_recv() {
  // RECEIVING
  if (rfm69.receiveDone()) // Got one!
  {
    // Print out the information:

    Serial.print("RFM69: received from node ");
    Serial.print(rfm69.SENDERID, DEC);
    Serial.print(", message [");

    // The actual message is contained in the DATA array,
    // and is DATALEN bytes in size:

    for (byte i = 0; i < rfm69.DATALEN; i++)
      Serial.print((char)rfm69.DATA[i]);

    // RSSI is the "Receive Signal Strength Indicator", smaller numbers mean higher power.

    Serial.print("], RSSI ");
    Serial.println(rfm69.RSSI);

    // Send an ACK if requested.
    if (rfm69.ACKRequested())
    {
      rfm69.sendACK();
      Serial.println("ACK sent");
    }
  }
}

/* ---------- RFM9X SEND ---------- */
void RFM9x_send() {
  if(Serial.available() > 0) {
    char input = Serial.read();

    if (input != '\r' && input != '\n') // not a carriage return
    {
      sendbuffer[sendlength] = input;
      sendlength++;
    }

    // If the input is a carriage return, or the buffer is full:

    if ((input == '\r') || (input == '\n') || (sendlength == 61)) // CR or buffer full
    {
      radio_cs = !radio_cs;
      // Send the packet!
      Serial.print("RFM9x: sending to node ");
      Serial.print(TONODEID, DEC);
      Serial.print(", message [");
      for (byte i = 0; i < sendlength; i++)
        Serial.print(sendbuffer[i]);
      Serial.println("]");

      Serial.println("Sending...");
      rfm9x.send((uint8_t *)sendbuffer, sendlength);

      sendlength = 0;
      sendbuffer[0] = '\0';
    }
  }
  rfm9x.waitPacketSent();
}

/* ---------- RFM9X RECEIVE ---------- */
void RFM9x_recv() {
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rfm9x.waitAvailableTimeout(100))
  { 
    // Should be a reply message for us now   
    if (rfm9x.recv(buf, &len))
   {
      Serial.print("RFM9x: Got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rfm9x.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
  else
  {
    //Serial.println("No reply, is there a listener around?");
  }
}
