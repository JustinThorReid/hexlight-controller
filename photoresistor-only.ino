
#include <avr/sleep.h>
#include <avr/power.h>

#include "PatternController.h"

#define PRESISTOR_PIN A0
#define RANDOM_PIN A2

#define MIN_LIGHT 30

volatile int f_timer=0;
PatternController *ledController;

void setup() {
  // For debugging
  Serial.begin(9600);
  setupSleep();
  
  // put your setup code here, to run once:
  randomSeed(analogRead(RANDOM_PIN));
  ledController = new PatternController();

  // Initialize sensor pins
  pinMode(PRESISTOR_PIN, INPUT);

  Serial.println("SETUP DONE");
  activeStateStart();
}

int pValue = 0;
unsigned long startMillis = 0;

#define MODE_ACTIVE true
#define MODE_WAIT false
bool waitMode;

#define ROLLING_COUNT 100
uint8_t activeRollingValues[ROLLING_COUNT];
uint8_t activeRollingIndex = 0;
uint8_t activeRollingCount = 0;

void loop() {  
  if(waitMode) {
    activeState();
  } else {
    waitState();
  }
}

void activeStateStart() {
  waitMode = MODE_ACTIVE;
  Serial.println("Active");

  activeRollingIndex = 0;
  activeRollingCount = 0;
  for(int i = 0; i < ROLLING_COUNT; i++) {
    activeRollingValues[i] = 0;
  }

  startMillis = millis();
  ledController->setType(3);
  Serial.println("Set type done");
}

void activeState() {  
  pValue = analogRead(PRESISTOR_PIN);
  activeRollingValues[activeRollingIndex] = pValue;
  activeRollingIndex = (activeRollingIndex + 1) % ROLLING_COUNT;
  if(activeRollingIndex > activeRollingCount) activeRollingCount = activeRollingIndex;

  // Get average light value
  float avg = 0;
  int16_t pMax = 0;
  int16_t pMin = 255;
  for(int i = 0; i < activeRollingCount; i++) {
    avg += activeRollingValues[i];

    if(activeRollingValues[i] > pMax) pMax = activeRollingValues[i];
    if(activeRollingValues[i] < pMin) pMin = activeRollingValues[i];
  }
  avg = avg / (float)activeRollingCount;

  // If below min light the turn off
  if(avg < MIN_LIGHT) {      
    waitStateStart();
    return;
  }

  // Keep the min/max range at least MIN_LIGHT wide
  if(pMax - pMin < MIN_LIGHT) {
    int diff = MIN_LIGHT - (pMax - pMin);    
    pMax += diff/2;
    pMin -= diff/2;
  }

  ledController->tick(millis() - startMillis);
}

void waitStateStart() {
  waitMode = MODE_WAIT;
  ledController->unset();
}

void waitState() {  
  if(f_timer==1)
  {
    f_timer = 0;
    
    pValue = analogRead(PRESISTOR_PIN);
    if(pValue > MIN_LIGHT) {
      activeStateStart();
      return;
    }
    
    /* Re-enter sleep mode. */
    enterSleep();
  }
}

/*** Configure the timer.***/  
void setupSleep() {
  /* Normal timer operation.*/
  TCCR1A = 0x00; 
  
  /* Clear the timer counter register.
   * You can pre-load this register with a value in order to 
   * reduce the timeout period, say if you wanted to wake up
   * ever 4.0 seconds exactly.
   */
  TCNT1=0x0000; 
  
  /* Configure the prescaler for 1:1024, giving us a 
   * timeout of 4.09 seconds.
   */
  TCCR1B = 0x05;
  
  /* Enable the timer overlow interrupt. */
  TIMSK1=0x01;
}

void enterSleep(void)
{
  set_sleep_mode(SLEEP_MODE_IDLE);  
  sleep_enable();

  /* Disable all of the unused peripherals. This will reduce power
   * consumption further and, more importantly, some of these
   * peripherals may generate interrupts that will wake our Arduino from
   * sleep!
   */
  power_adc_disable();
  power_spi_disable();
  power_timer0_disable();
  power_timer2_disable();
  power_twi_disable();  

  /* Now enter sleep mode. */
  sleep_mode();
  
  /* The program will continue from here after the timer timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */
  
  /* Re-enable the peripherals. */
  power_all_enable();
}

// Timer interrupt
ISR(TIMER1_OVF_vect)
{
  /* set the flag. */
   if(f_timer == 0)
   {
     f_timer = 1;
   }
}
