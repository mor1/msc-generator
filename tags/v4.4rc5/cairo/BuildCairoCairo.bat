set PATH=%PATH%;C:\Program Files\msys\bin
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\vsvars32.bat"

set ROOTDIR=%CD%\..

set INCLUDE=%INCLUDE%;%ROOTDIR%\zlib
set INCLUDE=%INCLUDE%;%ROOTDIR%\libpng
set INCLUDE=%INCLUDE%;%ROOTDIR%\pixman\pixman
set INCLUDE=%INCLUDE%;%ROOTDIR%\cairo\boilerplate
set INCLUDE=%INCLUDE%;%ROOTDIR%\cairo\cairo\src

set LIB_SAVE=%LIB%

set LIB=%LIB_SAVE%;%ROOTDIR%\zlib\projects\visualc71\Win32_LIB_Debug\Zlib
set LIB=%LIB%;%ROOTDIR%\libpng\projects\visualc71\Win32_LIB_Debug
make -f Makefile.win32 "CFG=debug" "LIBCFLAG=-MD"

set LIB=%LIB_SAVE%;%ROOTDIR%\zlib\projects\visualc71\Win32_LIB_Release\Zlib
set LIB=%LIB%;%ROOTDIR%\libpng\projects\visualc71\Win32_LIB_Release
make -f Makefile.win32 "CFG=release" "LIBCFLAG=-MD"

pause