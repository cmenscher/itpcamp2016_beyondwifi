


// rf69_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client
// with the RH_RF69 class. RH_RF69 class does not provide for addressing or
// reliability, so you should only use RH_RF69  if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf69_server.
// Demonstrates the use of AES encryption, setting the frequency and modem 
// configuration
// Tested on Moteino with RFM69 http://lowpowerlab.com/moteino/
// Tested on miniWireless with RFM69 www.anarduino.com/miniwireless
// Tested on Teensy 3.1 with RF69 on PJRC breakout board

#include <SPI.h>
#include <RH_RF69.h>

// Arduino Uno LED is on pin 13 but we can't use that pin b/c it's needed by SPI
#define LED 9 // Moteinos and Anarduinos have LED on D9

// Singleton instance of the radio driver
#define RF69_FREQ 433.0 //transmit frequency in MHz
#define RF69_TXPOWER 14 //dbM of transmit power, default to 14

//If you need to use custom pins for Slave Select and Payload Ready Interrupt, do this:
//#define RF69_SLAVE_SELECT_PIN 3 // default is 10, but some Arduinos like the TinyLily don't provide this pin.
//#define RF69_DIO0_PAYLOAD_READY_INTERRUPT_PIN 2 // default is 2
//RH_RF69 rf69(RF69_SLAVE_SELECT_PIN, RF69_DIO0_PAYLOAD_READY_INTERRUPT_PIN);

// Use the defaults to setup the RFM69
RH_RF69 rf69;


void setup() 
{

  Serial.begin(9600);

  pinMode(LED, OUTPUT);
  
  if (!rf69.init()) {
    Serial.println("RFM69 init failed");
    Serial.println("*** CRASH! ***");
    while(1);
  } else {
    Serial.println("RFM69 initialized!");
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM, No encryption
  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("setFrequency failed");
  } else {
    Serial.print("RFM69 Frequency: ");
    Serial.print(RF69_FREQ);
    Serial.println("MHz");
  }

  // If you are using a high power RF69, you *must* set a Tx power in the
  // range 14 to 20 like this:
  rf69.setTxPower(RF69_TXPOWER);
  Serial.print("Transmit power set to +" );
  Serial.print(RF69_TXPOWER);
  Serial.println(" dbM");

  // The encryption key has to be the same across all RFM69's you want to talk to
//  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
//                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
//  rf69.setEncryptionKey(key);
//
//#if 0
//  // For compat with RFM69 Struct_send
//  rf69.setModemConfig(RH_RF69::GFSK_Rb250Fd250);
//  rf69.setPreambleLength(3);
//  uint8_t syncwords[] = { 0x2d, 0x64 };
//  rf69.setSyncWords(syncwords, sizeof(syncwords));
//  rf69.setEncryptionKey((uint8_t*)"thisIsEncryptKey");
//#endif

  Serial.println("I am a transmitter node!");
  Serial.println("Ready..........FIGHT!");
}
}


void loop()
{
  Serial.println("Sending message to base station...");
  // Send a message to rf69_server
  uint8_t data[] = "Hello World!";
  rf69.send(data, sizeof(data));
  Serial.println("Message sent...\n");
//  Blink(LED, 1000);
  
  rf69.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf69.waitAvailableTimeout(500)) { 
    // Should be a reply message for us now   
    if (rf69.recv(buf, &len)) {
      Serial.print("got reply: ");
      Serial.println((char*)buf);
      for(int i=0; i<6; i++) {
        Blink(LED, 100);       
      }
    } else {
      Serial.println("recv failed");
    }
  } else {
    Serial.println("No reply, OMG WHAT HAPPENED TO THE BASE STATION?\n");
  }
  delay(400);
}

void Blink(byte PIN, int DELAY_MS) {
  digitalWrite(PIN,HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN,LOW);
  delay(DELAY_MS);
}


