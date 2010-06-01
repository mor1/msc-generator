echo This will delete!!!
pause

set PATH=%PATH%;C:\Program Files\msys\bin;C:\Program Files\curl
call "C:\Program Files\Microsoft Visual Studio .NET 2003\Common7\Tools\vsvars32.bat"

set ROOTDIR=C:\Users\ethzrt\Documents\BuildCairo-1.8.8-VC2003
rmdir /S /Q %ROOTDIR%\cairo
rmdir /S /Q %ROOTDIR%\libpng
rmdir /S /Q %ROOTDIR%\pixman
rmdir /S /Q %ROOTDIR%\zlib
mkdir %ROOTDIR%
cd %ROOTDIR%

tar -xzf zlib.tgz
tar -xzf libpng.tgz
tar -xzf pixman.tgz
tar -xzf cairo.tgz
move zlib-* zlib
move libpng-* libpng
move pixman-* pixman
move cairo-* cairo
pause

mkdir %ROOTDIR%\zlib\projects\visualc71
cd %ROOTDIR%\zlib\projects\visualc71
copy %ROOTDIR%\libpng\projects\visualc71\zlib.vcproj .

cd %ROOTDIR%\cairo
sed s/zdll.lib/zlib.lib/ build\Makefile.win32.common > build\Makefile.fixed
move /Y build\Makefile.fixed build\Makefile.win32.common
