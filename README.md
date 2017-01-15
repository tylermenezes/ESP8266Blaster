# ESP8266Blaster

IR Blasters are devices which can emulate remote controls, useful for home automation (or "automation," if you're one
of those people who just controls each of your lightbulbs from your smartphone). Unfortunately they're really expensive
for no particular reason: the cheapest I've seen is $60, and the fancy networked ones are usually $120+.

The ESP8266 is a wifi-enabled microcontroller which is usually available for $3-5 (or less, if you buy the surface
mount chips directly) and you can easily create a sub-$8 IR blaster by attaching an IR LED and a resistor.

This code exposes a network service on the ESP8266 which will allow you to send remote commands from a remote computer.
Sending IR remote data from your home automation system may be slightly more complicated than sending, say, `/tv/on`,
but it means you don't need to reprogram your microcontroller each time you add a new device.

## Installation and Flashing

This code uses Arduino's language/libraries because someone had already written an IR library, and I didn't want to
duplicate that work in LUA. You'll need to configure Arduino's IDE to talk to the ESP8266.

You'll also need to install [IRremoteESP8266](https://github.com/markszabo/IRremoteESP8266) into your Arduino libraries
folder.

Once that's set, you can configure your wifi network information and, if desired, change the IR pin and port in the
code, then just upload it.

`example.py` contains an example program for sending commands to a Denon AV receiver, because that's what I use this
for. You can find other IR codes online (search for IR Remote Database), but note that you'll need to provide raw
data and carrier frequencies, while most databases will list codes in NEC Codes or Pronto Hex. You can use a program
like [IrScrutinizer](http://harctoolbox.org/IrScrutinizer.html) to find and convert codes for you.

## Network Protocol

The protocol for communicating is very simple: send a byte stream to the device's IP, on the port configured in the
code, as follows:

- 1 byte: unsigned short (0-255) carrier frequency. This is usually 37 or 38.
- 2 bytes: unsigned short (0-65535) length of the IR data.
- n bytes: stream of two-byte-long shorts (0-65535) of raw IR data.

The length field should be the number of bytes remaining in the packet (not the number of shorts, or the total packet
size).

Once the device gets data, it won't respond to other clients until the command has been sent, or it times out waiting
for the full data (1sec).

This is not particularly secure. I have a separate VLAN for home automation devices which can only communicate over
whitelisted ports to my home automation server. I wouldn't recommend putting this on a public IP, although I don't
think anyone could do anything _really_ bad.

See `example.py` if you have any questions.
