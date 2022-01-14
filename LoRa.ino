



#include <SPI.h>
#include <LoRa.h>
#include "board_def.h"
#define Freq  403500000



OLED_CLASS_OBJ display(OLED_ADDRESS, OLED_SDA, OLED_SCL);

//display size for better string placement
int width;
int height;

void setup()
{
  pinMode(CONFIG_NOT_NSS, OUTPUT);
  digitalWrite(CONFIG_NOT_NSS, HIGH); // Init NSS for not work module LORA
  pinMode(25, OUTPUT);             // Init OUT for Green LED

  if (OLED_RST > 0) 
  {
    pinMode(OLED_RST, OUTPUT);
    digitalWrite(OLED_RST, HIGH);
    delay(100);
    digitalWrite(OLED_RST, LOW);
    delay(100);
    digitalWrite(OLED_RST, HIGH);
  }

  display.init();
  width = display.getWidth() / 2;
  height = display.getHeight() / 2;
  display.flipScreenVertically();
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(width - 50, height, LORA_SENDER ? "LoRa++ Sender" : "LoRa++ Receiver");
  display.display();
  delay(1000);


  SPI.begin(CONFIG_CLK, CONFIG_MISO, CONFIG_MOSI, CONFIG_NSS);
  LoRa.setPins(CONFIG_NSS, CONFIG_RST, CONFIG_DIO0);
  if (!LoRa.begin(BAND))
  {
    display.clear();
    display.drawString(width - 50, height, "Starting LoRa failed!");
    display.display();
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setFrequency(Freq);
  LoRa.setSpreadingFactor(8);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(7);
  LoRa.setPreambleLength(12);
  LoRa.setGain(0);
  LoRa.disableCrc();


  display.clear();
  display.drawString(width - 50, height, "LoraRecv++ Ready");
  display.display();
}
  void loop()
  {
    if (LoRa.parsePacket())
    {

      digitalWrite(25, HIGH);
      delay(200);
      digitalWrite(25, LOW);
      delay(200);

      char i = 0;
      String recv = "";
      while (LoRa.available())
      {
        recv += (char)LoRa.read();
        i++;
      }

      float lat = 0, lon = 0, humidity_float = 0;
      unsigned long int serial_number = 0, height_meter = 0;
      unsigned short int hum, temp;

      memcpy(&serial_number, &recv[0], 4);             // serial number
      memcpy(&height_meter, &recv[16], 2);             // height
      memcpy(&lon, &recv[24], 4);                      // logitude
      memcpy(&lat, &recv[28], 4);                      // latitude
      memcpy(&hum, &recv[12], 2);                      // humidity
      humidity_float = hum;                            // type data change for humiditye
      memcpy(&temp, &recv[10], 2);                     // temperature.
                 
      String number = "Serial number: " + String(serial_number);
      String height_string = "Height: " + String(height_meter) + "m   " + "RSSI: " + String(LoRa.packetRssi());
      String longitude = "Longitude:   " + String(lon, 6);
      String latitude =  "Latitude:    " + String(lat, 6);
      String hum_temp = "hum: " + String(humidity_float / 10, 1) + "% " " temp: " + String(temp);

      display.clear();
      display.drawString(width - 65, height + 16, hum_temp);
      display.drawString(width - 65, height - 0, height_string);
      display.drawString(width - 65, height - 16, longitude);
      display.drawString(width - 65, height - 32, latitude);
      display.display();


    }
  }
