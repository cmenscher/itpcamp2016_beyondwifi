// rf69_server.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing server
// with the RH_RF69 class. RH_RF69 class does not provide for addressing or
// reliability, so you should only use RH_RF69  if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf69_client
// Demonstrates the use of AES encryption, setting the frequency and modem
// configuration.
// Tested on Moteino with RFM69 http://lowpowerlab.com/moteino/
// Tested on miniWireless with RFM69 www.anarduino.com/miniwireless
// Tested on Teensy 3.1 with RF69 on PJRC breakout board

#include <SPI.h>
#include <RH_RF69.h>

#define LED 9 // Arduino Uno LED is on pin 13, Moteinos and Anarduinos have LED on D9

// Singleton instance of the radio driver
#define RF69_FREQ 433.0 //transmit frequency in MHz
#define RF69_TXPOWER 10 //dbM of transmit power, default to 14

//If you need to use custom pins for Slave Select and Payload Ready Interrupt, do this:
//#define RF69_SLAVE_SELECT_PIN 3 // default is 10, but TinyLily doesn't provide this pin.
//#define RF69_DIO0_PAYLOAD_READY_INTERRUPT_PIN 2 // default is 2
//RH_RF69 rf69(RF69_SLAVE_SELECT_PIN, RF69_DIO0_PAYLOAD_READY_INTERRUPT_PIN);

// Use the defaults to setup the RFM69
RH_RF69 rf69;

void setup() 
{
  Serial.begin(9600);

  pinMode(LED, OUTPUT);


  if (!rf69.init()) {
    Serial.println("init failed");
  } else {
    Serial.println("RFM69 initialized!");
  }
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  // No encryption

  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("setFrequency failed");
  } else {
    Serial.print("RFM69 Frequency: ");
    Serial.print(RF69_FREQ);
    Serial.println("MHz");
  }

  // If you are using a high power RF69, you *must* set a Tx power in the
  // range 14 to 20 like this:
  // rf69.setTxPower(14); 
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
  Serial.println("I am an RFM69 base station!");
  Serial.println("Ready..........FIGHT!");

  for(int i=0; i<10; i++) {
    Blink(LED, 100);       
  }
}

void loop()
{
  if (rf69.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf69.recv(buf, &len))
    {
      Serial.print("Message received: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.print(rf69.lastRssi(), DEC);
      Serial.println("dbM");

      for(int i=0; i<2; i++) {
        Blink(LED, 250);       
      }

      //delay(250);
      // Send a reply
      uint8_t data[] = "And hello back to you";
      rf69.send(data, sizeof(data));
      rf69.waitPacketSent();
      Serial.println("Sent a reply\n");
//      Blink(LED, 1000);
    }
    else
    {
      Serial.println("recv failed");
    }
  }
}


void Blink(byte PIN, int DELAY_MS) {
  digitalWrite(PIN,HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN,LOW);
  delay(DELAY_MS);
}


