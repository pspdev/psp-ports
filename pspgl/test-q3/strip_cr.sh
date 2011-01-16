#!/bin/sh
echo "strip CR/LF: $1"
sed -e "s///g" $1 > $1.bak && mv $1.bak $1

