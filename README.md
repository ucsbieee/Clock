# 7-segment wall clock stuff

Not sure when the clock was made or where the source code is, but a copy of the flash and fuses of the ATmega328P is preserved here (EEPROM is empty).

## Adjusting the time

The buttons on the clock aren't connected and don't do anything. Took me a while to figure that out...

The time on the RTC chip (DS1307) doesn't seem to match what is actually displayed on the clock, so figured the easiest way to adjust the time is to add a correcting offset to the current time stored in the chip. Included is a low-effort sketch to do that.

To update the time, take a picture of the clock so you can exactly determine the necessary correction offset based on the creation date of the photo. Enter this into the Arduino sketch (negative is back in time, positive forward) and upload it to any Arduino. Unplug the clock and remove the RTC board. Connect the RTC to power and the I2C bus on the Arduino. Open the serial monitor to see messages, and short pin 3 of the Arduino to ground when you're ready to update the time.

## Notes

9/21/22 — Found the clock had fallen off the wall. The fall seems to have shorted the top right transistor, which burnt that corner of the board until it failed short. Replaced it and the clock is back to normal.
