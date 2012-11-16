set PATH=%PATH%;C:\Program Files\msys\bin
call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\Tools\vsvars32.bat"

set ROOTDIR=%CD%\..

cd %ROOTDIR%\pixman\pixman
make -f Makefile.win32 "CFG=debug"
make -f Makefile.win32 "CFG=release"
