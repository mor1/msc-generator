set PATH=%PATH%;C:\Program Files\msys\bin
call "C:\Program Files\Microsoft Visual Studio 12.0\Common7\Tools\vsvars32.bat"

set ROOTDIR=%CD%\..

cd %ROOTDIR%\pixman-0.26.0\pixman
make -f Makefile.win32 "CFG=debug"
make -f Makefile.win32 "CFG=release"
