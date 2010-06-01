
set PATH=%PATH%;C:\Program Files\msys\bin;C:\Program Files\curl

set ROOTDIR=C:\Users\ethzrt\Documents\BuildCairo-1.8.8-VC2003
curl http://www.zlib.net/zlib-1.2.3.tar.gz -o zlib.tgz -x proxy.eth.ericsson.se:8080
curl http://prdownloads.sourceforge.net/libpng/libpng-1.2.40.tar.gz -o libpng.tgz -x proxy.eth.ericsson.se:8080
curl http://www.cairographics.org/releases/pixman-0.16.2.tar.gz -o pixman.tgz -x proxy.eth.ericsson.se:8080
curl http://www.cairographics.org/releases/cairo-1.8.8.tar.gz -o cairo.tgz -x proxy.eth.ericsson.se:8080

pause