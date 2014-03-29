#!/bin/bash

fpath=$(pwd)"/rec/"
fname="rec_single_TEST."
fext=".wav"

arecord -D plughw:1 --duration=2 -f cd -vv $fpath$fname$(date +"%Y:%m:%d:%S:%N")$fext

