#!/bin/bash
export SOUND_BASE_DIR=/home/pi/sounds
export YAAFE=$SOUND_BASE_DIR/yaafe-v0.64
export YAAFE_PATH=$YAAFE/yaafe_extensions
export PATH=$PATH:$YAAFE/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$YAAFE/lib
export PYTHONPATH=$PYTHONPATH:$YAAFE/python_packages
(cd $SOUND_BASE_DIR && ./raraa)
