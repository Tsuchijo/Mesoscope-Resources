# TM-Lab-Mesoscope-Resources

## Lens Adapters

All the lens adapters are parametrically created using this onshape file:

https://cad.onshape.com/documents/2f0def30531779d17ee9e510/w/bf5702dd5ef345ce03ccb7df/e/dbd37cfbbbade1fbfb717a2f?renderMode=0&uiState=656536ea70c36e66cdf41a41

![Image of the adapter parameters](image.png)

Adjust these parameters to change the adapters parameters, the diameter is the inner diameter of the lens, pitch is usually 0.5 or 0.75

## Strobing Trigger

### Circuit Diagrams
![Alt text](image-1.png)


### Triggering Unit 
![Sub unit diagram](image-2.png)

### Routing 
![Alt text](image-3.png)

### I/O
![io diagram](io_diagram.png)

For the Master Clock in / power in pin out from left to right is 
- master clock
- ground
- power (5v)

for Timing signal the pinout is
- Brain timing control
- Behavior timing control
- Strobing timing control

with the switches controlling in the same order.

In order to control the width of the timing signal supply a pwm signal to each of the width control signals, with the width corresponding to change in signal. The supplied signal should be greater than 1khz and ideally as high frequency as possible to reduce artifacts

the timing and output pots allow for manual control of timing and output voltage, although each of them can cause errors if pushed to their limits due to variation in their ranges from manufacturing. Never turn the timing pots all the way to the right as that might cause a short

### Using

To use supply a 50hz PWM signal to the master clock with as wide a PWM as possible, the width of the signal corresponding to the maximum possible exposure time.

Supply a 5khz PWM signal for each width control pin in to control the exposure for brain and the behaviour cameras independently. Make sure the switch is switched to ON for the control pin being used. The Width of the PWM signal corresponds with the timing, with the widest signal being the longest exposure time.

### Control Example Code
```import RPi.GPIO as GPIO
import time

# Set GPIO mode and warning
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

# Define the pin numbers
master_clock_pin = 17
width_control_pin = 18

# Set the frequency for the master clock (50Hz)
master_clock_frequency = 50

# Set the duty cycle for the master clock (99% width)
master_clock_duty_cycle = 99

# Set the frequency for the width control (5kHz)
width_control_frequency = 5000

# Initialize the GPIO pins
GPIO.setup(master_clock_pin, GPIO.OUT)
GPIO.setup(width_control_pin, GPIO.OUT)

# Initialize PWM for master clock
master_clock_pwm = GPIO.PWM(master_clock_pin, master_clock_frequency)
master_clock_pwm.start(master_clock_duty_cycle)

# Function to control the width of the second PWM
def set_width(shutter_speed):
    width_percentage = shutter_speed * 100
    width_control_pwm.ChangeDutyCycle(width_percentage)

# Initialize PWM for width control
width_control_pwm = GPIO.PWM(width_control_pin, width_control_frequency)
width_control_pwm.start(0)  # Start with 0% duty cycle

try:
    # Example: Set shutter speed to 0.5 (50% duty cycle)
    shutter_speed = 0.5
    set_width(shutter_speed)

    # Keep the program running
    while True:
        time.sleep(1)

except KeyboardInterrupt:
    # Clean up on program exit
    master_clock_pwm.stop()
    width_control_pwm.stop()
    GPIO.cleanup()
```
