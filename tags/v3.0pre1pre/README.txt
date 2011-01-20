This is the windows directory of msc-generator.

I have included a copy the required libraries (zlib, libpng, pixman and cairo) in the repo. 
I know it is ugly, but
- I need one bugfix to cairo
- Changed compilation options

So for anyone, who wants to compile msc-generator for windows, this is the easiest.

To compile, you need Visual C++ 2010 and gnu make. 

First compile cairo.sln both for "Debug" and "Release" (firts one will return an error, that is OK).
This will (collectively) compile both the debug and release cairo.dll (including my fix)

Then you can compile Msc-generator.sln. It will always include the release version of cairo.dll.

Enjoy and send feedback.

Z