#!/usr/bin/python
# Spawn pseudo-tty for input testing.
# Copyright 2010, Canonical, Ltd.
# Author: Kees Cook <kees@ubuntu.com>
# License: GPLv3
import os
import sys
import select

parent, child = os.openpty()
tty = os.ttyname(child)
os.system('stty cs8 -icanon -echo < %s' % (tty))

print tty

try:
    os.system('stty cs8 -icanon -echo < /dev/stdin')

    rlist = [parent, sys.stdin]

    running = True
    while running:
        ready, _, _ = select.select(rlist, [], [])
        for fd in ready:
            chars = os.read(fd, 512)
            if fd == parent:
                sys.stdout.write(chars)
                sys.stdout.flush()
            else:
                os.write(parent, chars)
finally:
    os.system('stty sane < /dev/stdin')
