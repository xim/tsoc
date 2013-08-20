# The Smell of Coffee in the Morning

**The idea**:  
Everybody likes waking up to the smell of coffee. Nobody would want to wake up
without it, given a choice. Now, with as little effort as possible, you can
have this every day – Just buy an Arduino, a Raspberry Pi, a 16-key keypad, an
PCD8544 based LCD, and some 5v relays... oh, and a coffee maker! You'll be
waking up happy 'till the end of your days.


## Design draft, Arduino-centered design

**Basics**:  
The Arduino does all the heavy lifting, but storage and accurate time is on the
Pi. At boot, the Arduino requests and waits for SPI time sync. When this
succeeds, the device is in normal operation, and requests an alarm sync.

Possible messages from the Arduino:

    \t – Request a time update
    \v – Request that the time be spoken
    \n – Make a really annoying sound at random
    \a – request that all alarm data be resent
    (Also, remaining messages apply here)

Possible messages from the Pi:

    T(timestamp) – Set the time
    P(actionno)(actionspec){8} – Set programmable alarm action with action specs
    A(alarmno)(actionno){8} – Set alarm actions for given alarm
    O(alarmno)(timestamp) – Set alarm to run once at given time stamp
    N(alarmno)(string) – Set name/text for given alarm
    R(alarmno)(repeatspec) – Set alarm to repeat at repeat spec.
    D(alarmno) – Delete alarm at given number

Specifications for message contents:

    timestamp:  A 4-byte time_t, unix time stamp. Time zones are not supported.
    actionno:   1 byte, an action identifier
    actionspec: (actionmask)(actionargs)
    actionmask: unit32_t, a mask of available actions (see below)
    actionargs: Four bytes determining snooze behaviour of the alarm:
                first bit sets snoozability (bool),
                next 15 bits set timeout (uint16_t),
                final 16 bits set offset (int16_t)
    alarmno:    1 byte, an alarm identifier
    string:     ASCII string, 12 chars max.
    repeatspec: 1 Byte, determiming repetition behaviour (see below)


Available actions for actionmask:

     Byte 1   Byte 2   Byte 3   Byte 4
    |bbbbbbbb|bbbbbbbb|bbbbbbbb|bbbbbbbb|
     ││││││││ ││││││││ ││││││││ │││││││└ Enable relay one
     ││││││││ ││││││││ ││││││││ ││││││└─ Disable relay one
     ││││││││ ││││││││ ││││││││ │││││└── Enable relay two
     ││││││││ ││││││││ ││││││││ ││││└─── Disable relay two
     ││││││││ ││││││││ ││││││││ │││└──── Enable relay three
     ││││││││ ││││││││ ││││││││ ││└───── Disable relay three
     ││││││││ ││││││││ ││││││││ │└────── Enable relay four
     ││││││││ ││││││││ ││││││││ └─────── Disable relay four

     ││││││││ ││││││││ │││││││└ ──────── Enable backlight
     ││││││││ ││││││││ ││││││└─ ──────── Blink backlight at 1s intervals
     ││││││││ ││││││││ │││││└── ──────── Output a really annoying sound at random
     ││││││││ ││││││││ ││││└─── ──────── Speaking clock, 1 minute interval
     ││││││││ ││││││││ │││└──── ──────── Speaking clock, 2 minute interval
     ││││││││ ││││││││ ││└───── ──────── Speaking clock, 3 minute interval
     ││││││││ ││││││││ │└────── ──────── Speaking clock, 5 minute interval
     ││││││││ ││││││││ └─────── ──────── Speaking clock, 10 minute interval

     ││││││││ │││││││└ ──────── ──────── Dimmer control 1 on, 5 min
     ││││││││ ││││││└─ ──────── ──────── Dimmer control 1 on, 3 min
     ││││││││ │││││└── ──────── ──────── Dimmer control 1 on, 1 min
     ││││││││ ││││└─── ──────── ──────── Dimmer control 1 on, 10 sec
     ││││││││ │││└──── ──────── ──────── Reserved for future use
     ││││││││ ││└───── ──────── ──────── Reserved for future use
     ││││││││ │└────── ──────── ──────── Reserved for future use
     ││││││││ └─────── ──────── ──────── Reserved for future use

     │││││││└ ──────── ──────── ──────── Reserved for future use
     ││││││└─ ──────── ──────── ──────── Reserved for future use
     │││││└── ──────── ──────── ──────── Reserved for future use
     ││││└─── ──────── ──────── ──────── Reserved for future use
     │││└──── ──────── ──────── ──────── Reserved for future use
     ││└───── ──────── ──────── ──────── Reserved for future use
     │└────── ──────── ──────── ──────── Reserved for future use
     └─────── ──────── ──────── ──────── Reserved for future use


Available repetition specs for repeatspec:

     Byte 1
    |bbbbbbbb|
     │││││││└ Mondays
     ││││││└─ Tuesdays
     │││││└── Wednesdays
     ││││└─── Thursdays
     │││└──── Fridays
     ││└───── Saturdays
     │└────── Sundays
     └─────── Reserved for future use (Geek days?)
