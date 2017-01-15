#!/usr/bin/env python2
import struct
import socket

# Turn On
# command = (264, 792, 264, 1848, 264, 792, 264, 792, 264, 792, 264, 1848, 264, 792, 264, 792, 264, 792, 264, 792, 264, 1848, 264, 1848, 264, 1848, 264, 792, 264, 792, 264, 43560, 264, 792, 264, 1848, 264, 792, 264, 792, 264, 792, 264, 792, 264, 1848, 264, 1848, 264, 1848, 264, 1848, 264, 792, 264, 792, 264, 792, 264, 1848, 264, 1848, 264, 43560)

# Turn Off
command = (264, 792, 264, 1848, 264, 792, 264, 792, 264, 792, 264, 792, 264, 1848, 264, 792, 264, 792, 264, 792, 264, 1848, 264, 1848, 264, 1848, 264, 792, 264, 792, 264, 43560, 264, 792, 264, 1848, 264, 792, 264, 792, 264, 792, 264, 1848, 264, 792, 264, 1848, 264, 1848, 264, 1848, 264, 792, 264, 792, 264, 792, 264, 1848, 264, 1848, 264, 43560)

freq = 38

commandBytes = list(sum([struct.unpack('2B', struct.pack('H', i)) for i in command], ()))
lenBytes = struct.unpack('2B', struct.pack('H', len(commandBytes)))
packet = (((0, freq) + lenBytes + tuple(commandBytes))[1:])
print packet

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.settimeout(5)
s.connect(('10.129.254.251',31442))
s.send(bytearray(packet))
s.close()
