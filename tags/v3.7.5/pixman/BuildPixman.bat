set PATH=%PATH%;C:\Program Files\msys\bin
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\vsvars32.bat"

set ROOTDIR=%CD%\..

cd %ROOTDIR%\pixman\pixman
make -f Makefile.win32 "CFG=debug" "LIBCFLAG=-MD"
make -f Makefile.win32 "CFG=release" "LIBCFLAG=-MD"

pause