# The Smell of Coffee in the Morning

**The idea**:  
Everybody likes waking up to the smell of coffee. Nobody would want to wake up
without it, given a choice. Now, with as little effort as possible, you can
have this every day – Just buy an Arduino, a Raspberry Pi, a 16-key keypad, an
PCD8544 based LCD, and some 5v relays... oh, and a coffee maker! You'll be
waking up happy 'till the end of your days.


## Design – Arduino-centered design

**Basics**:  
The Arduino does all the heavy lifting, but storage and accurate time is on the
Pi. At boot, the Arduino requests and waits for SPI time sync. When this
succeeds, the device is in normal operation, and requests an alarm sync.

**Operation**:  
The Arduino is the master in this, and requests actions from the Pi. These
requests include playing sound, storing data, or replaying all alarm data from
storage. To see details of the actions passed, see libcwap.h – any other source
is not likely to be updated.
