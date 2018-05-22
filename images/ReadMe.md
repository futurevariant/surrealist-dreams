# Wiring

The relay has been prewired to the various LED's.

The arduino does not power the relay; the relay is opto-isolated and powered via a USB cable with the black header pin connected to ground, and the red header pin connected to JD-VCC. 
The header used is a 3 pin header with only pins 1 and 3 wired so black goes to ground and red to JD-VCC. 

On the mega, pins 10-3 are connected to pins 1 - 8 accordingly; 10->1, 9->2 ... 3->8. The wiring strip has all 10 wires connected together for convenience

The 5V output of the arduino is connected to the VCC pin on the relay inpus (right at the end of the long header).

GND on the long header is not connected as the Arduino does not power the relay.
