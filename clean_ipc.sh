#!/bin/sh

shm=$(ipcs -m | grep 0x00000888 | awk '{print $2}')
if [ "$shm" != "" ]
then
  ipcrm -m $shm
fi

sem=$(ipcs -s | grep 0x00000548 | awk '{print $2}')
if [ "$sem" != "" ]
then
  ipcrm -s $sem
fi

