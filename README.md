# buttonpusher
Device to push a button in our office.

# Description

As a modern office person, I want the ability to push a button remotely, so that I do not need to get out of my seat, or need to get my wallet out.

# Implementation Details - MVP

Servo mounted to bluetooth MCU. Gets triggered by either detecting a... (TBD)

* Physical button (at least)
* New bluetooth device via mobile phone?
* Web Bluetooth + a Progressive Web App? 

We should consider using a mechanism (MOSFET N-Channel might work) to try and cut power to the servo after it has been restored to 'normal' position.

http://bildr.org/2012/03/rfp30n06le-arduino/

We have a couple options on bluetooth + MCU boards.

### RedBear Nano (v2)

I believe to be replaced by the Xenon?

https://store.particle.io/products/redbear-nano-v2-with-headers

### Particle Xenon

https://www.particle.io/mesh/


# Considerations

* Do not block the button (i.e. so cannot use a solenoid)
* Low battery usage (1+ month?)
* MVP to use equipment from 'Innovation Lab'

# Acknowledgements

Universal Button Pusher (Thingiverse): https://www.thingiverse.com/thing:2806324
