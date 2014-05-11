#!/bin/bash


##Install any depednant libraries for the recording executable
echo "RECORD_SETUP: Installing dependant libraries..."

DEPENDANT_LIBS="cmake cmake-curses-gui libargtable2-0 libargtable2-dev libsndfile1 libsndfile1-dev libmpg123-0 libmpg123-dev libfftw3-3 libfftw3-dev liblapack-dev libhdf5-serial-dev libhdf5-7 python flac"

sudo apt-get install $DEPENDANT_LIBS


##Yaafe must already be installed manually!!


##Create the recording executable
echo "RECORD_SETUP: Creating recording executable..."

COMPILER="g++"
RECORD_FILE_NAME="makeRecord.cpp"
RECORD_EXEC_NAME="mr"
COMP_FLAGS="-o"

$COMPILER $RECORD_FILE_NAME $COMP_FLAGS $RECORD_EXEC_NAME