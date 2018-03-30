#!/bin/bash

if [ ! -d ../build ]
then
  echo "Need to build first"
  exit -1
else
  ../bin/cpptext
fi
exit
