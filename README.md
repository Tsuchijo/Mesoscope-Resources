# TM-Lab-Mesoscope-Resources

## Lens Adapters

All the lens adapters are parametrically created using this onshape file:

https://cad.onshape.com/documents/2f0def30531779d17ee9e510/w/bf5702dd5ef345ce03ccb7df/e/dbd37cfbbbade1fbfb717a2f?renderMode=0&uiState=656536ea70c36e66cdf41a41

![Image of the adapter parameters](image.png)

Adjust these parameters to change the adapters parameters, the diameter is the inner diameter of the lens, pitch is usually 0.5 or 0.75

## Strobing Trigger

### Circuit Diagrams
![Alt text](image-1.png)


### Triggerign Unit 
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