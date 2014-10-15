#!/bin/bash

echo " === SETTING START === "

export SOUND_BASE_DIR=`pwd`
export HOME_BIN="$HOME/bin/"


echo " === (1/3) START - Recording envrionment is being prepared  === "

echo "Installing dependant libraries..."
DEPENDANT_LIBS="cmake cmake-curses-gui libargtable2-0 libargtable2-dev libsndfile1 libsndfile1-dev libmpg123-0 libmpg123-dev libfftw3-3 libfftw3-dev liblapack-dev libhdf5-serial-dev libhdf5-7 python flac alsa-utils"


case "$OSTYPE" in
    darwin*)  
	echo "OS is OSX"
	sudo port install $DEPENDANT_LIBS    
	echo "Done."
	sudo port update
	echo "Done." ;;
    linux*)   
	echo "OS is LINUX"
	sudo apt-get -y install $DEPENDANT_LIBS
	echo "Done."
	
	echo "Updating RPi software..."
	sudo apt-get -y update
	sudo apt-get -y upgrade
	sudo apt-get -y install rpi-update
	sudo rpi-update
	echo "Done." ;;	
    *)        
	echo "WARNING: OS '$OSTYPE' UNSUPPORTED. Continuing assuming the following dependant libraries are installed and updated: '"+$DEPENDANT_LIBS+"'." ;;
esac



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
echo " ... impliment me more ... "
echo "Done."


echo "Extracting Yaafe ... "

export YAAFE_NAME="yaafe-v0.64"
export YAAFE=$SOUND_BASE_DIR/$YAAFE_NAME

if [ ! -d "$YAAFE" ]; then
    echo "Yaafe directory not found locally! Creating one ... "
    if [ ! -f $YAAFE.tgz ]; then
	echo "Yaafe tar ball not found locally! Downloading Yaafe ... "
        wget -O $YAAFE_NAME.tgz https://sourceforge.net/projects/yaafe/files/$YAAFE_NAME.tgz/download
    fi
    echo "Extracting yaafe files ... "
    tar -xf $YAAFE.tgz
    echo "Done."
fi
echo "Done."

echo "Building Yaafe ... "
cd $YAAFE/
if [ ! -f built ]; then
    echo "Yaafe being built for the first time ... "
    ccmake -DCMAKE_INSTALL_PREFIX=. . #install everything to local yaafe
    #ccmake #configure and generate
    sudo make
    sudo make install
    touch built
fi
cd -
echo "Done."


#To easily use Yaafe, you should set the following environment vars:
export YAAFE_PATH=$YAAFE/yaafe_extensions
export PATH=$PATH:$YAAFE/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$YAAFE/lib
export PYTHONPATH=$PYTHONPATH:$YAAFE/python_packages

echo "Listing important Yaafe paths ... "
echo "YAAFE = $YAAFE"
echo "PATH = $PATH"
echo "LD_LIBRARY_PATH = $LD_LIBRARY_PATH"
echo "PYTHONPATH = $PYTHONPATH"
echo "Done."

echo " === (2/3) STOP - Audio feature extraction envrionment setup  === "





echo " === (3/3) START - Preparing raraa === "

echo "cleaning up old workspace..."
make clean
echo "Done."

echo "preparing new workspace..."
make
echo "Done."

export SOUND_BASE_DIR=`pwd`

echo "Creating 'start' script... "
export START_SOUND="start_sound.sh" 
touch $START_SOUND
chmod u+x $START_SOUND

echo "#!/bin/bash"                                            > $START_SOUND
echo "export SOUND_BASE_DIR=`pwd`"                            >> $START_SOUND
echo "export YAAFE=\$SOUND_BASE_DIR/yaafe-v0.64"              >> $START_SOUND
echo "export YAAFE_PATH=\$YAAFE/yaafe_extensions"             >> $START_SOUND
echo "export PATH=\$PATH:\$YAAFE/bin"                         >> $START_SOUND
echo "export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:\$YAAFE/lib"   >> $START_SOUND
echo "export PYTHONPATH=\$PYTHONPATH:\$YAAFE/python_packages" >> $START_SOUND
echo "(cd \$SOUND_BASE_DIR && ./raraa)"                       >> $START_SOUND

if [ -d "$HOME_BIN" ]; then
    cp $START_SOUND $HOME_BIN
    echo "Copied '$START_SOUND' to '$HOME_BIN'."
fi
echo "Done."



echo "Creating 'stop' script... "
export STOP_SOUND="stop_sound.sh"
touch $STOP_SOUND
chmod u+x $STOP_SOUND

echo "#!/bin/bash"                     > $STOP_SOUND
echo "echo \" ... impliment me ... \"" >> $STOP_SOUND

if [ -d "$HOME_BIN" ]; then
    cp $STOP_SOUND $HOME_BIN
    echo "Copied '$STOP_SOUND' to '$HOME_BIN'."
fi
echo "Done."


echo "Creating 'clean_workspace' script... "
export CLEAN_LOCAL="clean_workspace.sh"
touch $CLEAN_LOCAL
chmod u+x $CLEAN_LOCAL

echo "#!/bin/bash"       > $CLEAN_LOCAL
echo "cd `pwd`/analysis" >> $CLEAN_LOCAL
echo "rm rec*"           >> $CLEAN_LOCAL
echo "cd `pwd`/data"     >> $CLEAN_LOCAL
echo "rm rec*"           >> $CLEAN_LOCAL
echo "Done."


echo " === (3/3) STOP - Prepared raraa === "





echo " === SETUP COMPLETE === "

