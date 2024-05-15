#include <RadioLib.h>

// nRF24 has the following connections:
// CS pin:    10
// IRQ pin:   2
// CE pin:    3
nRF24 radio = new Module(10, 2, 3);

// or using RadioShield
// https://github.com/jgromes/RadioShield
// nRF24 radio = RadioShield.ModuleA;

void setFlag();

void setup()
{
  Serial.begin(115200);

  // initialize nRF24 with default settings
  Serial.print(F("[nRF24] Initializing ... "));
  int state = radio.begin();
  if (state == RADIOLIB_ERR_NONE)
  {
    Serial.println(F("success!"));
  }
  else
  {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true)
      ;
  }

  // set transmit address
  // NOTE: address width in bytes MUST be equal to the
  //       width set in begin() or setAddressWidth()
  //       methods (5 by default)
  byte addr[] = {0x01, 0x23, 0x45, 0x67, 0x89};
  Serial.print(F("[nRF24] Setting transmit pipe ... "));
  state = radio.setTransmitPipe(addr);
  if (state == RADIOLIB_ERR_NONE)
  {
    Serial.println(F("success!"));
  }
  else
  {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true)
      ;
  }
}

// counter to keep track of transmitted packets
int count = 0;

bool switch1 = false;
bool switch2 = false;
bool switch3 = false;
bool switch4 = false;

#define SWITCH1 A3
#define SWITCH2 A0
#define SWITCH3 6
#define SWITCH4 7

void loop()
{
  switch1 = digitalRead(SWITCH1);
  switch2 = digitalRead(SWITCH2);
  switch3 = digitalRead(SWITCH3);
  switch4 = digitalRead(SWITCH4);

  Serial.print(F("[nRF24] Transmitting packet ... "));

  String str = String(switch4) + String(switch3) + String(switch2) + String(switch1);
  int state = radio.transmit(str);

  if (state == RADIOLIB_ERR_NONE)
  {
    Serial.println(F("success!"));
  }
  else if (state == RADIOLIB_ERR_PACKET_TOO_LONG)
  {
    Serial.println(F("too long!"));
  }
  else if (state == RADIOLIB_ERR_ACK_NOT_RECEIVED)
  {
    Serial.println(F("ACK not received!"));
  }
  else if (state == RADIOLIB_ERR_TX_TIMEOUT)
  {
    Serial.println(F("timeout!"));
  }
  else
  {
    Serial.print(F("failed, code "));
    Serial.println(state);
  }
  delay(100);
}