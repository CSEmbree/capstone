#!/bin/bash

echo " === SETTING START === "

export SOUND_BASE_DIR=`pwd`


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
touch start_sound.sh
echo "#!/bin/bash"                                            > start_sound.sh
echo "export SOUND_BASE_DIR=`pwd`"                            >> start_sound.sh
echo "export YAAFE=\$SOUND_BASE_DIR/yaafe-v0.64"              >> start_sound.sh
echo "export YAAFE_PATH=\$YAAFE/yaafe_extensions"             >> start_sound.sh
echo "export PATH=\$PATH:\$YAAFE/bin"                         >> start_sound.sh
echo "export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:\$YAAFE/lib"   >> start_sound.sh
echo "export PYTHONPATH=\$PYTHONPATH:\$YAAFE/python_packages" >> start_sound.sh
echo "(cd \$SOUND_BASE_DIR && ./raraa)"                       >> start_sound.sh
echo "Done."

echo "Creating 'stop' script... "
touch stop_sound.sh
echo "#!/bin/bash"                     > stop_sound.sh
echo "echo \" ... impliment me ... \"" >> stop_sound.sh
echo "Done."

echo "Creating 'clean_workspace' script... "
touch clean_workspace.sh
echo "#!/bin/bash"       > clean_workspace.sh
echo "cd `pwd`/analysis" >> clean_workspace.sh
echo "rm rec*"           >> clean_workspace.sh
echo "cd `pwd`/data"     >> clean_workspace.sh
echo "rm rec*"           >> clean_workspace.sh
echo "Done."

echo "ensuring executables..."
chmod u+x install.sh
chmod u+x start_sound.sh
chmod u+x stop_sound.sh
chmod u+x clean_workspace.sh
echo "Done."


echo " === (3/3) STOP - Prepared raraa === "





echo " === SETUP COMPLETE === "

