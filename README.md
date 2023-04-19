# IT8951 1bpp A2_MODE emulator

Small utility for developing graphic interfaces for the IT8951 EPD driver, in particular
with the 10.3" Waveshare display. This is a quick and dirty tool made to be able to visualize
my development without carrying a screen taped to styrofoam on the train :)

# The display and data load alignment

While testing out modifications of the sample code I run into problems when the X px coordinates
of an image load do not align with 4 bytes. The controller seems to be rounding the coordinates
up to the nearest 4 byte alignment. Depending on how many bits per pixel you are using, this results
in different pixel alignments. The image width must also be 4 byte padded, otherwise it will also get
rounded up and the resulting image will be deformed (photos to come).

For now the emulator only supports 1bpp (bits-per-pixel) because it is anyway the only bpp setting
supported by the A2 mode for fast partial updates of the screen.

While the image load x coordinate needs to be 4 byte algined, the x coordinate for displaying
(revealing a portion of the image buffer in the screen) can be anything.

# Requirements

For the window and pixel painting, the program requires SDL2 libs installed in the system.

I've only used this under linux, shouldn't be too hard to run it on any other platform.

# Limitations

For now, the limitations are:

  - Only one screen possible
  - Only 1bpp images
  - No timing emulation, everything will be loaded asap and displayed when called asap. Adding timing emulation is probably my next step after I finish the GUI library.

# Example

You can run `make example` to generate an example binary that draws a square on a screen.

The code for that is in [src/example.c](src/example.c)

# Tests

There's some small unit tests included that I used to make sure the behaviour was correct while developing.

I included unity as a submodule so if you want to run `make test` make sure to init and update the git submodules.