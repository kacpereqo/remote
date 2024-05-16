
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RadioLib.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
nRF24 radio = new Module(10, 2, 3);

void setup()
{

  Serial.begin(115200);
  while (!Serial)
    ;

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    while (true)
      ;
  }

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

bool switch1 = false;
bool switch2 = false;
bool switch3 = false;
bool switch4 = false;

constexpr int SWITCH1 = A0;
constexpr int SWITCH2 = A3;
constexpr int SWITCH3 = 6;
constexpr int SWITCH4 = 7;

void loop()
{
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(24, 12);
  display.cp437(true);

  Serial.print(F("[nRF24] Transmitting packet ... "));

  switch1 = digitalRead(SWITCH1);
  switch2 = digitalRead(SWITCH2);
  switch3 = digitalRead(SWITCH3);
  switch4 = digitalRead(SWITCH4);

  String str = String(switch4) + String(switch3) + String(switch1) + String(switch2);
  int state = radio.transmit(str);

  if (state == RADIOLIB_ERR_NONE)
  {
    // the packet was successfully transmitted
    Serial.println(F("success!"));
  }
  else if (state == RADIOLIB_ERR_PACKET_TOO_LONG)
  {
    // the supplied packet was longer than 32 bytes
    Serial.println(F("too long!"));
  }
  else if (state == RADIOLIB_ERR_ACK_NOT_RECEIVED)
  {
    // acknowledge from destination module
    // was not received within 15 retries
    display.println("Connection NIE OK");
    Serial.println(F("ACK not received!"));
  }
  else if (state == RADIOLIB_ERR_TX_TIMEOUT)
  {
    // timed out while transmitting
    Serial.println(F("timeout!"));
  }
  else
  {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);
  }

  if (state != RADIOLIB_ERR_ACK_NOT_RECEIVED)
  {
    display.println("Connection OK");
  }

  display.setCursor(50, 24);
  display.print(str);

  display.display();

  delay(1000 / 200);
}
