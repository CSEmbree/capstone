#!/bin/bash

echo " === SETTING UP - START === "



echo "Installing dependant libraries..."
DEPENDANT_LIBS="cmake cmake-curses-gui libargtable2-0 libargtable2-dev libsndfile1 libsndfile1-dev libmpg123-0 libmpg123-dev libfftw3-3 libfftw3-dev liblapack-dev libhdf5-serial-dev libhdf5-7 python flac"
sudo apt-get install $DEPENDANT_LIBS
echo "Done."


echo "Updating RPi software..."
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install rpi-update
sudo rpi-update
echo "Done."


echo "Adding user pi to the audio group..."
sudo usermod -a -G audio pi
echo "Done."


echo "Listing visible sound cards..."
lsusb
echo "Done."


echo "Setting 'amixer' recording settings..."
amixer -c 1 cset numid=7,iface=MIXER,name='Auto Gain Control' 0
numid=7,iface=MIXER,name='Auto Gain Control'
echo "Storing recording settings..."
sudo alsactl store 1
echo "Done"


echo "Checking 'arecord' version..."
arecord --version
echo "Done."


echo "Making sure sound card is visible..."
arecord -l
echo "Done."


echo " === Recordings can now be made. === "


echo "Making sure sound card is visible..."
export YAAFE_PATH=~/capstone/yaafe-v0.64/yaafe_extensions
export PATH=$PATH:~/capstone/yaafe-v0.64/bin
tar -xf yaafe-v0.64.tgz 
cd yaafe-v0.64
ccmake
make
make install
touch built
cd -

echo " === SETTING UP - STOP === "

