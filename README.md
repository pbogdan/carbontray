# carbontray

A very simple, standalone Linux system tray based on Budgie Desktop tray implementation. Please see
https://github.com/pbogdan/libcarbontray for more details.

Presently it is unlikely it would satisfy needs any other than those of the author.

It does fix one long standing annoyance of mine - an issue I often encountered with other
implementations were tray icons drawing over themselves creating distorted tray icons and all sort
of visual mess. One prime example of that is the NetworkManager applet - changing signal strength,
animation when connecting to a network were all causing visual glitches.

This has been fixed by Budgie Desktop developers in the carbontray library (thank you!) and this
project is a simple (and presently dumb) wrapper around their code.

## Usage

Run `carbontray --help` to see the list of available options.

__Important__: any of the options that specify sizes/dimensions accept values GTK calls application
pixels. That is - the values will be multiplied by GTK's scaling factor. For example specifying
width of 100 will result in effective width of 200 when the scaling factor is set to 2.

The appearance of the bar can be customised by placing `style.css` file under
`$XDG_CONFIG_HOME/carbontray/` (typically ~/.config/carbontray). For example changing the background
can be done with the following:

```
#carbontray {
  background-color: rgba(0x38, 0x3a, 0x42, 0.59);
}
```
