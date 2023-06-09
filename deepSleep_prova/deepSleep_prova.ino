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

MKRIoTCarrier carrier;

void setup() {
  Serial.begin(9600);
  carrier.begin();
  // Uncomment this function if you wish to attach function dummy when RTC wakes up the chip
  //LowPower.attachInterruptWakeup(RTC_ALARM_WAKEUP, dummy, CHANGE);
}

void loop() {
  /*
  carrier.leds.setPixelColor(0,0,255,0);
  carrier.leds.show();
  */

  carrier.display.fillScreen(ST77XX_WHITE);
  carrier.display.setTextColor(ST77XX_BLACK);
  carrier.display.setTextSize(2);

  carrier.display.setCursor(25, 110);
  carrier.display.println("avvio");

  delay(2000);
  
  carrier.display.fillScreen(ST77XX_RED);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(2);

  carrier.display.setCursor(25, 110);
  carrier.display.println("avvio");

  // Triggers a 2000 ms sleep (the device will be woken up only by the registered wakeup sources and by internal RTC)
  // The power consumption of the chip will drop consistently

  LowPower.sleep(20000);
}

void dummy() {
  // This function will be called once on device wakeup
  // You can do some little operations here (like changing variables which will be used in the loop)
  // Remember to avoid calling delay() and long running functions since this functions executes in interrupt context
  carrier.leds.setPixelColor(0,255,255,0);
  carrier.leds.show();

  carrier.display.fillScreen(ST77XX_RED);
  carrier.display.setTextColor(ST77XX_BLACK);
  carrier.display.setTextSize(2);

  carrier.display.setCursor(25, 110);
  carrier.display.println("sveglio");
}