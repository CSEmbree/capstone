#!/bin/bash

echo " === SETTING START === "




echo " === (1/3) START - Recording envrionment is being prepared  === "

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

echo " === (1/3) STOP - Recording envrionment is setup  === "





echo " === (2/3) START - Audio feature extraction envrionment setup  === "

echo "Making sure sound card is visible..."
export YAAFE_PATH=~/sound/yaafe-v0.64/yaafe_extensions
export PATH=$PATH:~/sound/yaafe-v0.64/bin
tar -xf yaafe-v0.64.tgz 
cd yaafe-v0.64
ccmake
make
make install
touch built
cd -
echo "Done."


echo " ... impliment me more ... "

echo " === (2/3) STOP - Audio feature extraction envrionment setup  === "





echo " === (3/3) START - Preparing raraa === "

echo "cleaning up old workspace..."
make clean
echo "Done."

echo "preparing new workspace..."
make
echo "Done."

echo "Creating 'start' script... "
touch start.sh
echo "#!/bin/bash"                 > start.sh
echo "export SOUND_BASE_DIR=`pwd`" >> start.sh
echo "./raraa"                     >>  start.sh
echo "Done."

echo "Creating 'stop' script... "
touch stop.sh
echo "#!/bin/bash"                     > stop.sh
echo "echo \" ... impliment me ... \"" >> stop.sh
echo "Done."

echo "ensuring executables..."
chmod u+x install.sh
chmod u+x start.sh
chmod u+x stop.sh
chmod u+x clean_workspace.sh
echo "Done."


echo " === (3/3) STOP - Prepared raraa === "





echo " === SETUP COMPLETE === "

