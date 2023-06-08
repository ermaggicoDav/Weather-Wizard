#include <avr/sleep.h>

void setup()  {
  pinMode(13, OUTPUT);
  pinMode(2, INPUT);

  attachInterrupt(0, wakeUpNow, LOW);

  delay(3000);
  sleepNow();
}

void loop() {

}

void sleepNow() {
  digitalWrite(13, LOW);
  /*
  5 differenti modi di farlo dormire
  SLEEP_MODE_IDLE         -meno profondo
  SLEEP_MODE_ADC
  SLEEP_MODE_PWR_SAVE
  SLEEP_MODE_STANBY
  SLEEP_MODE_PWR_DOWN     -più salva energia
  */
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  attachInterrupt(0, wakeUpNow, LOW);
  sleep_mode();

  sleep_disable();
  
}