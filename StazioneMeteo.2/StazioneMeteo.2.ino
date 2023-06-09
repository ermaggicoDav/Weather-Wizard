#include <AirQualityClass.h>
#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_MKRIoTCarrier_Buzzer.h>
#include <Arduino_MKRIoTCarrier_Qtouch.h>
#include <Arduino_MKRIoTCarrier_Relay.h>
#include <EnvClass.h>
#include <IMUClass.h>
#include <MKRIoTCarrierDefines.h>
#include <PressureClass.h>

#include "ArduinoLowPower.h"


const int sensoreUmiditaTerreno = A5; //indica il pin del sensore

MKRIoTCarrier carrier;

//bool CARRIER_CASE = false;
float temperature = 0;
float humidity = 0;
float pressure = 0;
float soilMoisture = 0;


const int interval = 30000;  //tempo tra le rilevazioni
const int tSpento = 20000;  //tempo che rimane spento
const int tShow = 700;  //tempo in cui si vede a schermo le misure
unsigned long previousMillis = 0;


void setup() {
  Serial.begin(9600);
  while(!Serial); //questo fa iniziare solo se c'è la seriale, quindi se siamo collegato a pc
  carrier.begin();
  pinMode(sensoreUmiditaTerreno, INPUT);

  Serial.println("  Temp  |  Humi  |   Press  |   Soil   |  time ");


}

void loop() {

  carrier.leds.clear();
  carrier.leds.show();

  carrier.display.fillScreen(ST77XX_WHITE);
  carrier.display.setTextColor(ST77XX_GREEN);
  carrier.display.setTextSize(2);

  carrier.display.setCursor(75, 110);
  carrier.display.println("aspetto");

  temperature = carrier.Env.readTemperature();
  humidity = carrier.Env.readHumidity();
  pressure = carrier.Pressure.readPressure();
  soilMoisture = analogRead(sensoreUmiditaTerreno);

  carrier.Buttons.update();

  if(carrier.Button1.onTouchDown()) {
      misura();

      delay(200);
      
      printTemperature();
      delay(tShow);
      printHumidity();
      delay(tShow);
      printPressure();
      delay(tShow);
      printSoilMoisture();
      delay(tShow);
  }
  if(carrier.Button2.onTouchDown()) {
      scrive();

      delay(200);

      Serial.print(temperature);
      Serial.print(" °C, ");
      Serial.print(humidity);
      Serial.print(" %, ");
      Serial.print(pressure);
      Serial.print(" Pa, ");
      Serial.print(soilMoisture);
      Serial.print(" %, ");
      Serial.print(millis());
      Serial.println(" ms");
  }
  if(carrier.Button3.onTouchDown()) {
    spento();
    LowPower.sleep(tSpento);
    //delay(tSpento);
  }


}



void printTemperature() {
  carrier.display.fillScreen(ST77XX_RED);
  carrier.display.setTextColor(ST77XX_BLACK);
  carrier.display.setTextSize(2);

  carrier.display.setCursor(50, 100);
  carrier.display.print("Temperatura: ");
  carrier.display.setCursor(75, 120);
  carrier.display.print(temperature);
  carrier.display.print(" C");
}

void printHumidity()  {
  carrier.display.fillScreen(ST77XX_BLUE);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(2);

  carrier.display.setCursor(65, 100);
  carrier.display.print("Umidita': ");
  carrier.display.setCursor(75, 120);
  carrier.display.print(humidity);
  carrier.display.print(" %");
}

void printPressure()  {
  carrier.display.fillScreen(ST77XX_GREEN);
  carrier.display.setTextColor(ST77XX_MAGENTA);
  carrier.display.setTextSize(2);

  carrier.display.setCursor(40, 100);
  carrier.display.print("pressione atm: ");
  carrier.display.setCursor(65, 120);
  carrier.display.print(pressure);
  carrier.display.print(" Pa");
}

void printSoilMoisture()  {
  carrier.display.fillScreen(ST77XX_ORANGE);
  carrier.display.setTextColor(ST77XX_BLUE);
  carrier.display.setTextSize(2);

  carrier.display.setCursor(20, 100);
  carrier.display.print("umidita' terreno: ");
  carrier.display.setCursor(70, 120);
  carrier.display.print(soilMoisture);
  carrier.display.print(" %");
}


//mette i led gialli quando misura
void misura() {
  carrier.leds.clear();
  carrier.leds.setBrightness(20);

  //carrier.leds.setPixelColor(index, red, green, blue);
  carrier.leds.setPixelColor(0,255,255,0);
  carrier.leds.setPixelColor(1,255,255,0);
  carrier.leds.setPixelColor(2,255,255,0);
  carrier.leds.setPixelColor(3,255,255,0);
  carrier.leds.setPixelColor(4,255,255,0);

  carrier.leds.show();

  carrier.display.fillScreen(ST77XX_YELLOW);
  carrier.display.setTextColor(ST77XX_BLACK);
  carrier.display.setTextSize(2);

  carrier.display.setCursor(25, 110);
  carrier.display.println("sta misurando...");
}

//spegne i led quando è fermo
void spento() {

  carrier.leds.clear();
  carrier.leds.setBrightness(20);

  //carrier.leds.setPixelColor(index, red, green, blue);
  carrier.leds.setPixelColor(2,0,0,255);


  carrier.leds.show();

  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(2);

  carrier.display.setCursor(80, 110);
  carrier.display.println("spento");
}

//mette i led a rosso quando scrive
void scrive() {
  carrier.leds.clear();
  carrier.leds.setBrightness(20);

  carrier.leds.setPixelColor(0,255,0,0);
  carrier.leds.setPixelColor(1,255,0,0);
  carrier.leds.setPixelColor(2,255,0,0);
  carrier.leds.setPixelColor(3,255,0,0);
  carrier.leds.setPixelColor(4,255,0,0);

  carrier.display.fillScreen(ST77XX_RED);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(2);
  carrier.display.setCursor(80, 110);
  carrier.display.println("scrive");

  carrier.leds.show();
}

void printToString()  {
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity = ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" Pa");

  Serial.print("Soil Moisture = ");
  Serial.print(soilMoisture);
  Serial.println(" %");
}
