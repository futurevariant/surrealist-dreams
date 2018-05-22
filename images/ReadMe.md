# Wiring

The relay has been prewired to the various LED's.

The arduino does not power the relay; the relay is opto-isolated and powered via a USB cable with the black header pin connected to ground, and the red header pin connected to JD-VCC. 
The header used is a 3 pin header with only pins 1 and 3 wired so black goes to ground and red to JD-VCC. 

On the mega, pins 10-3 are connected to pins 1 - 8 accordingly; 10->1, 9->2 ... 3->8. The wiring strip has all 10 wires connected together for convenience

The 5V output of the arduino is connected to the VCC pin on the relay inpus (right at the end of the long header).

GND on the long header is not connected as the Arduino does not power the relay.



# Wemos Board

The image diagram dippy.png shows the various dip switch combinations for programming the board

The board has been configured to allow the mega to be flashed AND to read serial data from the port, while the mega and the ESP8266 communicate via Hardware Serial 3. 

Dipswitches 1 - 4 need to be ON, and the large black switch should be on TX-RX3 (it only has two positions).

It can work fine standalone or be flashed/debugged in this mode

#Power

The board can be powered via a micro usb cable or a 9V 2A PSU. A hub has been included to power the relay and the board. 
