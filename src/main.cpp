#include "heltec.h"
#include "esp_system.h"
#define BAND 433E6 //you can se\t band here directly,e.g. 868E6,915E6
#define SPREADING_FACTOR 11

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

char x = 0;
char y = 0;

String msgString = "";
String incoming = "";

int msgCount = 1;

void initScreen()
{
  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->display();
}

void printScreen(String message)
{
  Serial.println(message);
  msgString += message;
}

void renderScreen()
{
  digitalWrite(LED, LOW);
  Heltec.display->setPixel(x, y);
  Heltec.display->display();
}

void printScreen()
{
  printScreen("");
}

void onReceive(int packetSize)
{
  Serial.println("packetSize: " + String(packetSize));
  digitalWrite(LED, HIGH);
  if (packetSize == 0)
    return;

  char incomingLength = LoRa.read();
  Serial.printf("incomingLength: %i\n", incomingLength);
  if (incomingLength != 255) {
    return;
  }

  x = LoRa.read();
  Serial.printf("X: %i\n", x);
  y = LoRa.read();
  Serial.printf("Y: %i\n", y);
  LoRa.read(); // pixel value
  Serial.println("");
}

void setup()
{
  setCpuFrequencyMhz(80);
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  LoRa.setSpreadingFactor(SPREADING_FACTOR);
  initScreen();

  renderScreen();
  LoRa.onReceive(onReceive);
  LoRa.receive();
}

const int INTERVAL = 1000;
int lastUpdate = millis();
int i = 0;

void loop()
{
  renderScreen();
}
