#!/bin/sh

CC=/opt/eldk-5.2.1/armv5te/sysroots/i686-eldk-linux/usr/bin/armv5te-linux-gnueabi/arm-linux-gnueabi-gcc
CFLAGS="-Wall -ansi"
LDFLAGS="-Wall"


echo --- clear o-Files ---
rm *.o -f
echo

echo --- clear out-Files ---
rm ./*.out -f
echo

echo --- Compile: adc_test.o ---
$CC $CFLAGS -c -o adc_test.o adc_test.c
echo

echo --- Linking: adc_test.out ---
$CC $LDFLAGS -o adc_test.out \
  adc_test.o \

echo