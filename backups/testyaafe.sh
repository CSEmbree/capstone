#!/bin/bash
export SOUND_BASE_DIR=/home/pi/sounds
export YAAFE=$SOUND_BASE_DIR/yaafe-v0.64
export YAAFE_PATH=$YAAFE/yaafe_extensions
export PATH=$PATH:$YAAFE/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$YAAFE/lib
export PYTHONPATH=$PYTHONPATH:$YAAFE/python_packages
yaafe.py -r 44100 -c /home/pi/sounds/featureplan_fin /home/pi/data/rec_D-26-8-114_T-16-41-1.wav
#yaafe.py -h
