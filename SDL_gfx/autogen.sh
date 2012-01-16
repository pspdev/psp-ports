#!/bin/sh
#

aclocal -I $(psp-config --psp-prefix)/share/aclocal
automake --foreign
autoconf

#./configure $*
echo "Now you are ready to run ./configure"
