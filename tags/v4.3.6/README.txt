This is the windows directory of msc-generator.

I have included a copy the required libraries (zlib, libpng, pixman and cairo) in the repo. 
I know it is ugly, but
- I need one bugfix to cairo
- Changed compilation options

So for anyone, who wants to compile msc-generator for windows, this is the easiest.

To compile, you need Visual C++ 2012 and gnu make. 

First compile cairo.sln both for "Debug" and "Release" (first one will return an error, that is OK).
This will (collectively) compile both the debug and release cairo.dll (including my fix)

However, you do not need to compile cairo, as I have included compiled binaries in the "compiled_cairo" 
directory. This is where Msc-generator.sln takes the compiled binary.

Then you can compile Msc-generator.sln. It will include the release or debug version of cairo.dll, as appropriate.

Enjoy and send feedback.

Z