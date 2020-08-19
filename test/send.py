#!/usr/bin/env python

import serial
import sys
from xmodem import XMODEM


def usage():
    print(f"{sys.argv[0]} <tty>")


if len(sys.argv) != 2:
    usage()
    sys.exit(1)

tty = sys.argv[1]
port = serial.Serial(tty, 9600)
modem = XMODEM(lambda size, timeout=1: port.read(size),
               lambda data, timeout=1: port.write(data))
with open("test-file.txt", "rb") as stream:
    modem.send(stream)

sys.exit(0)
