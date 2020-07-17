# carbontray

A very simple, standalone Linux system tray based on https://github.com/pbogdan/libcarbontray.

Presently it is unlikely it would satisfy needs any other than those of the author.

It does fix one long standing annoyance of mine - an issue I often encountered with other
implementations were tray icons drawing over themselves creating distorted tray icons and all sort
of visual mess. One prime example of that is the NetworkManager applet - changing signal strength,
animation when connecting to a network were all causing visual glitches.

This has been fixed by Budgie Desktop developers in the carbontray library (thank you!) and this
project is a simple (and presently dumb) wrapper around their code.
