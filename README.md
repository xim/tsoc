# The Smell of Coffee in the Morning

This branch is likely seeing abandonment – moving to a Raspberry Pi-oriented
design.


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
