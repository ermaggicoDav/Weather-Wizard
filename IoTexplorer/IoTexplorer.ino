#include <AirQualityClass.h>
#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_MKRIoTCarrier_Buzzer.h>
#include <Arduino_MKRIoTCarrier_Qtouch.h>
#include <Arduino_MKRIoTCarrier_Relay.h>
#include <EnvClass.h>
#include <IMUClass.h>
#include <MKRIoTCarrierDefines.h>
#include <PressureClass.h>


MKRIoTCarrier carrier;

//bool CARRIER_CASE = false;
float temperature = 0;
float humidity = 0;


void setup() {
  //Serial.begin(9600);
  //while(!Serial); //questo fa iniziare solo se c'è la seriale, quindi se siamo collegato a pc
  carrier.begin();


}

void loop() {
  temperature = carrier.Env.readTemperature();
  humidity = carrier.Env.readHumidity();

  carrier.Buttons.update();

  /*
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity = ");
  Serial.print(humidity);
  Serial.println(" %");
  */

  printTemperature();

  /*
  if(carrier.Button1.onTouchDown()) {
    printTemperature();
  }
  if(carrier.Button2.onTouchDown()) {
    printHumidity();
  }
  */
}

void printTemperature() {
  carrier.display.fillScreen(ST77XX_RED);
  carrier.display.setTextColor(ST77XX_BLACK);
  carrier.display.setTextSize(2);

  carrier.display.setCursor(20, 110);
  carrier.display.print("Temp: ");
  carrier.display.print(temperature);
  carrier.display.println(" °C");
}

void printHumidity()  {
  carrier.display.fillScreen(ST77XX_BLUE);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(2);

  carrier.display.setCursor(20, 110);
  carrier.display.print("Humi: ");
  carrier.display.print(humidity);
  carrier.display.println(" %");
}
