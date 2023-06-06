#include <BMx280I2C.h>
#include <BMx280MI.h>
#include <BMx280SPI.h>
#include <BMx280SPIClass.h>
#include <BMx280TwoWire.h>

#include <wire.h>

BMx280I2C bmx280(0x76);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();

  bmx280.begin();
  bmx280.resetToDefaults();

  bmx280.writeOversamplingPressure(BMx280I2C::OSRS_P_x16);
  bmx280.writeOversamplingTemperature(BMx280I2C::OSRS_T_x16);
  bmx280.writeOversamplingHumidity(BMx280I2C::OSRS_H_x16);

}

void loop() {
  // put your main code here, to run repeatedly:
  bmx280.mesure();
  do{
    delay(100);
  } while(!bmx280.hasValue());

  Serial.print("pressure: ");
  Serial.println(bmx280.getPressure());
  Serial.print("temperature: ");
  Serial.println(bmx280.getTemperature());
  Serial.print("humidity: ");
  Serial.println(bmx280.getHumidity());

  delay(1000);
}
