This is the windows directory of msc-generator.

I have included a copy the required libraries (zlib, libpng, pixman and cairo) in the repo. 
I know it is ugly, but
- I need one bugfix to cairo
- Changed compilation options

So for anyone, who wants to compile msc-generator for windows, this is the easiest.

To compile, you need Visual C++ 2008 SP1 and gnu make. Current settings give a stand-alone, 
statically linked exe, quite big, but I prefer this to depending on many MS dlls. (My ignorance,
but this is it.)

Enjoy and send feedback.

Z