#!/bin/bash

fpath=$(pwd)"/rec/"
fname="recTEST."
fext=".wav"

for i in `seq 1 30`;
  do
    arecord -D plughw:1 --duration=$i -f cd -vv $fpath$fname$(date +"%Y:%m:%d:%S:%N")$fext
  done 






