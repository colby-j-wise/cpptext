#!/bin/bash

if [ ! -d ../build ]
then
  echo "Need to build first"
  exit -1
else
  ../bin/cpptext --data_path="../data/blogs" --output_path="../output" --num_threads=8
fi
exit
