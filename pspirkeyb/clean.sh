#!/bin/sh
find . -name "*~" -print | xargs rm -f
rm -rf asciidemo/Debug
rm -rf irkeybtest/Debug
rm -rf libpspirkeyb/Debug
rm -f psp*.suo
rm -f psp*.ncb
rm -f asciidemo/*.vcproj.*
rm -f irkeybtest/*.vcproj.*
rm -f libpspirkeyb/*.vcproj.*
rm -f *.tgs
make clean
