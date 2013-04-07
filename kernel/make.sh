#!/bin/bash

cp -f lpc313x_adc.c ~/Arbeitsfläche/Gnublin/gnublin-develop-kernel/linux-3.3.0-lpc313x/drivers/misc/lpc31xx/lpc313x_adc.c
cp -f lpc313x_adc_dev.h ~/Arbeitsfläche/Gnublin/gnublin-develop-kernel/linux-3.3.0-lpc313x/drivers/misc/lpc31xx/lpc313x_adc_dev.h

cd ~/Arbeitsfläche/Gnublin/gnublin-develop-kernel/linux-3.3.0-lpc313x

make modules

if [ $? -gt 0 ]
then
  exit 1
fi

make modules_install INSTALL_MOD_PATH=~/Arbeitsfläche/Gnublin/Kernel/