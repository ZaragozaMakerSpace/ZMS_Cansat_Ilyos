
#include <SPI.h>
#include <RH_RF69.h>

// Singleton instance of the radio driver
RH_RF69 rf69(10, 3);

void setup() 
{
  Serial.begin(115200);
  while (!Serial) 
    ;
  if (!rf69.init())
    Serial.println("init failed");
  // Defaults modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
  if (!rf69.setFrequency(868.0))
    Serial.println("setFrequency failed");

  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  rf69.setTxPower(14, true);

  // For compat with RFM69 Struct_send
  rf69.setModemConfig(RH_RF69::GFSK_Rb250Fd250);
  Serial.println("Init Done");
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

      Serial.print("got request: ");
      Serial.println((char*)buf);
//      Serial.print("RSSI: ");
//      Serial.println(rf69.lastRssi(), DEC);
      
      // Send a reply
      // uint8_t data[] = "And hello back to you";
      // rf69.send(data, sizeof(data));
      // rf69.waitPacketSent();
      // Serial.println("Sent a reply");
    }
    else
    {
      Serial.println("recv failed");
    }
  }
}
