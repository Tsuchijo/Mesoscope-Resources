/**
* User Defined Parameters
**/
#define INTERRUPT_RATE_HZ 54
#define PRESCALER_VALUE 64 //Dont change this
#define USE_INTERNAL_TIMER true
const int inputPin = 2; // Assume PWM input is on 

const int outputPins[] = {8, 9, 10}; // PWM output on pin 8, 9, 10
const int strobePins[] = {6, 7}; //PWM ouput pins for 2 strobing outputs (must be 2)

// Each output width is the width of the pulse in millis on each clock cycle, paired by their position in the arraty
// e.g. pins[] = {1,2 } widths[] = {10, 15} gives a 10 millisecond pulse on 1 and a 15 millisecond pulse on 2
const float outputWidths[] = {16, 0.5, 10}; //Width of pulse output in milliseconds (shutter speed)
const float strobeWidth = 9; //Length of Strobing Pulse in Milliseconds


const int numOutput = sizeof outputPins / sizeof outputPins[0];
volatile bool fallingEdgeDetected = false;
volatile bool strobe = false;
long microsCount = 0;

void setup() {
  if (USE_INTERNAL_TIMER){
    initTimer1();
  }
  else {
    // Setup the input as an interrupt pin on ever falling edge
    pinMode(inputPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(inputPin), fallingEdgeInterrupt, FALLING);
  }
  // Specify all output pins
  for (int i = 0; i < numOutput; i++){
    pinMode(outputPins[i], OUTPUT);
  }
  pinMode(strobePins[0], OUTPUT);
  pinMode(strobePins[1], OUTPUT);



  // Enable global interrupts
  sei();
}

void loop() {
  //When the Interrupt is triggered turn on all outputs and reset the timer
  if (fallingEdgeDetected) {
    for (int i = 0; i < numOutput; i++){
      //Set output pins low since camera is triggered on low
      digitalWrite(outputPins[i], LOW);
    }
    //Set Strobing high
    if (strobe) {digitalWrite(strobePins[0], HIGH);}
    else digitalWrite(strobePins[1], HIGH);
    fallingEdgeDetected = false;  // Reset the flag
    microsCount = micros(); // reset the counter
  }

  //For each pin check if clock is over then reset
  for (int i = 0; i < numOutput; i++){
    if (outputWidths[i] * 1000  < micros() - microsCount){
      digitalWrite(outputPins[i], HIGH);
    }
  }
  if (strobeWidth * 1000 < micros() - microsCount){
    if (strobe) {digitalWrite(strobePins[0], LOW);}
    else digitalWrite(strobePins[1], LOW);
  }
}

// Function to initialize Timer1 for 50Hz interrupt
void initTimer1() {
  // Set the Timer1 Control Register A (TCCR1A) to 0
  TCCR1A = 0;

  // Set the Timer1 Control Register B (TCCR1B) to configure the timer
  // Use CTC (Clear Timer on Compare Match) mode
  // Set the prescaler to PRESCALER_VALUE
  TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10); 

  // Calculate the value to be loaded into the Timer1 register
  // Timer1 operates at 16MHz (Arduino Uno clock frequency)
  // We want a 50Hz interrupt, so the formula is: Timer_value = (Clock_freq / (Prescaler * Target_freq)) - 1
  uint16_t timer1Value = (16000000UL / (PRESCALER_VALUE * INTERRUPT_RATE_HZ)) - 1;

  // Set the Timer1 register (OCR1A) to the calculated value
  OCR1A = timer1Value;

  // Enable the Timer1 Compare A interrupt
  TIMSK1 |= (1 << OCIE1A);
}

void fallingEdgeInterrupt() {
  //detect interrupt
  fallingEdgeDetected = true;
  //Switch which pin is strobing 
  strobe = !strobe;
}

// Interrupt Service Routine (ISR) for Timer1 Compare A
ISR(TIMER1_COMPA_vect) {
  //detect interrupt
  fallingEdgeDetected = true;
  //Switch which pin is strobing 
  strobe = !strobe;
}


