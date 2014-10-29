#!/bin/bash

# README: This installation script attempts to: 
# 1. Identify a recording device
# 2. Update/install dependant libraries for recording audio and audio analysis with Yaafe 
# 3. Compile any libraries needed for audio recording/analysis
# 4. Create start and stop scripts for audio recording/analysis


# dependant libraries that need to be installed or updated if they do not exist
DEPENDANT_LIBS="cmake cmake-curses-gui libargtable2-0 libargtable2-dev libsndfile1 libsndfile1-dev libmpg123-0 libmpg123-dev libfftw3-3 libfftw3-dev liblapack-dev libhdf5-serial-dev libhdf5-7 python flac alsa-utils"

DO_LOCAL=false
DO_UPDATE=true
DO_COMPILE=true

export SOUND_BASE_DIR=`pwd`
export HOME_BIN="$HOME/bin/"






echo " === INSTALLATION START === "






echo " === (1/4) START - Prepapring for installation  === "

# Check for OPTIONAL argument parameters 
# argument code from (http://stackoverflow.com/questions/192249/how-do-i-parse-command-line-arguments-in-bash)
for i in "$@"
do
    case $i in
	-l|--local)
	    DO_LOCAL=true
	    shift
	    ;;
	-nu|--noupdate)
	    DO_UPDATE=false
	    shift
	    ;;
	-nc|--nocompile)
	    DO_COMPILE=false
	    shift
	    ;;
	*)
    # unknown option
	    echo "WARNING: Unrecognized argument '$key' !"
	    ;;
    esac
done




# let user know what install steps are going to happen
echo "************************************************"
echo "Install script will:"

if [ "$DO_UPDATE" = true ] ; then
    echo " -- Update dependant libraries: $DEPENDANT_LIBS"
fi

echo " -- Create start and stop scripts locally. "

if [ "$DO_COMPILE" = true ] ; then
    echo " -- Build Raraa. "
else 
    echo " -- NOT build Raraa anew"
fi

if [ "$DO_LOCAL" = false ] ; then
    echo " -- Copy start and stop scripts to '$HOME_BIN'. "
fi
echo "************************************************"

echo " === (1/4) STOP - Finished preparing for installation  === "






echo " === (2/4) START - Recording envrionment is being prepared  === "

# Update depedant libraries before compiling anything that might use them (user can turn this off by argument)
if [ "$DO_UPDATE" = true ] ; then
    echo "Installing/updating dependant libraries ... "
    
    case "$OSTYPE" in
	darwin*)  
	    # install dependant libs
	    echo "OS is OSX"
	    sudo port install $DEPENDANT_LIBS    
	    echo "Done."

	    # update OS/ports
	    sudo port update
	    echo "Done." ;;
	linux*)
	    # install dependant libs
	    echo "OS is LINUX"
	    sudo apt-get -y install $DEPENDANT_LIBS
	    echo "Done."
	    
	    # update OS
	    echo "Updating RPi software ... "
	    sudo apt-get -y update
	    sudo apt-get -y upgrade
	    sudo apt-get -y install rpi-update
	    sudo rpi-update
	    echo "Done." ;;	
	*)        
	    echo "WARNING: OS '$OSTYPE' UNSUPPORTED. Continuing assuming the following dependant libraries are installed and updated: '"+$DEPENDANT_LIBS+"'." ;;
    esac
fi


# prepare audio environment
echo "Adding current user to the audio group ... "
sudo usermod -a -G audio pi
echo "Done."

echo "Listing visible sound cards ... "
lsusb
echo "Done."

echo "Setting 'amixer' recording settings ... "
amixer -c 1 cset numid=7,iface=MIXER,name='Auto Gain Control' 0
numid=7,iface=MIXER,name='Auto Gain Control'
echo "Storing recording settings ... "
sudo alsactl store 1
echo "Done."

echo "Checking 'arecord' version ... "
arecord --version
echo "Done."

echo "Making sure sound card is visible ... "
arecord -l
echo "Done."

echo " === (2/4) STOP - Recording envrionment is setup  === "






echo " === (3/4) START - Audio feature extraction envrionment setup  === "

echo "Making sure sound card is visible..."
echo " ... impliment me ... "
echo "Done."


echo "Detecting Yaafe ... "
# assume Yaafe does not exist on machine by default
YAAFE_EXISTED=false

# Check if minimum Yaafe install requirments are already in path
# minimum Yaafe requirments are the existance of "YAAFE_PATH" (http://yaafe.sourceforge.net/manual/install.html#building-on-unix-macosx)
if [ ! -d "$YAAFE_PATH" ]; then

    # Yaafe was not already in the path, check for it locally
    YAAFE_EXISTED=false
    export YAAFE_NAME="yaafe-v0.64"
    export YAAFE=$SOUND_BASE_DIR/$YAAFE_NAME
    
    # Look for local install directory of Yaafe
    if [ ! -d "$YAAFE" ]; then
	# Yaafe was not found locally so we need to extract and build it from a tarball
	echo "Yaafe directory not found locally! Creating one ... "

	# Look for local tarball
	if [ ! -f $YAAFE.tgz ]; then
	    # The Yaafe tarball was not locally, get it from online
	    echo "Yaafe tar ball not found locally! Downloading Yaafe ... "
            wget -O $YAAFE_NAME.tgz https://sourceforge.net/projects/yaafe/files/$YAAFE_NAME.tgz/download
	fi

	# Extract the local Yaafe tarball
	echo "Extracting yaafe files ... "
	tar -xf $YAAFE.tgz
	echo "Done."
    fi
    echo "Done."
    

    # A Yaafe install directory exists locally, check it if it has not been built/compiled already
    echo "Building Yaafe ... "
    cd $YAAFE/


    # Build check by placing a 'built' empty file in yaafe after first compilation
    if [ ! -f built ]; then
	echo "Yaafe being built for the first time ... "
	cmake -DCMAKE_INSTALL_PREFIX=. . #install everything to local yaafe
        #ccmake #configure and generate
	sudo make
	sudo make install
	touch built # built flag for future reference that we have already built Yaafe
    fi
    cd -
    echo "Done."

    #To easily use Yaafe locally, we should set the following environment vars:
    export YAAFE_PATH=$YAAFE/yaafe_extensions
    export PATH=$PATH:$YAAFE/bin
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$YAAFE/lib
    export PYTHONPATH=$PYTHONPATH:$YAAFE/python_packages
else
    # If Yaafe exists in the envrionment paths, we assume all paths are correct and handled by someone else (nice!)
    echo "YAAFE found! (YAAFE_PATH = $YAAFE_PATH)."
    YAAFE_EXISTED=true
    echo "Assuming: PATH, LD_LIBRARY_PATH are setup for Yaafe. Assuming PYTHONPATH exists."
fi



# Sanity check on environment
echo "Important Yaafe paths are as follows ... "
echo "YAAFE install directory = $YAAFE"
echo "PATH = $PATH"
echo "LD_LIBRARY_PATH = $LD_LIBRARY_PATH"
echo "PYTHONPATH = $PYTHONPATH"
echo "Done."

echo " === (3/4) STOP - Audio feature extraction envrionment setup  === "






echo " === (4/4) START - Preparing raraa === "

# Prepare to compile Raraa and create start and stop scripts for Raraa
if [ "$DO_COMPILE" = true ]; then
    echo "cleaning up old workspace ... "
    #make clean
    echo "Done."
    
    echo "Preparing Raraa ... "
    make
    echo "Done."
fi


# Ensure a /home/bin/ directory exists if user wants start/stop scripts copied there
if [ "$DO_LOCAL" = false ]; then
    echo "Checking if a '$HOME_BIN' directory exists for sound start and stop scripts."

    if [ ! -d "$HOME_BIN" ]; then
	mkdir $HOME_BIN
	echo "Created '$HOME_BIN' for the first time."
    else
	echo "'$HOME_BIN' already exists."
    fi
    
    echo "Done."
fi


# Create a start script to ensure correct envrionment before running Raraa
echo "Creating 'start' script ... "
export START_SOUND="start_sound.sh" 
touch $START_SOUND
chmod u+x $START_SOUND

echo "#!/bin/bash"                                                > $START_SOUND
echo "export SOUND_BASE_DIR=`pwd`"                                >> $START_SOUND
# If yaafe was not already setup, then add the local yaafe install paths to the run script
if [ "$YAAFE_EXISTED" = false ]; then
    echo "export YAAFE=\$SOUND_BASE_DIR/$YAAFE_NAME"              >> $START_SOUND
    echo "export YAAFE_PATH=\$YAAFE/yaafe_extensions"             >> $START_SOUND
    echo "export PATH=\$PATH:\$YAAFE/bin"                         >> $START_SOUND
    echo "export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:\$YAAFE/lib"   >> $START_SOUND
    echo "export PYTHONPATH=\$PYTHONPATH:\$YAAFE/python_packages" >> $START_SOUND
fi
echo "(cd \$SOUND_BASE_DIR && ./raraa)"                           >> $START_SOUND

if [ "$DO_LOCAL" = false ]; then
    cp $START_SOUND $HOME_BIN
    echo "Copied start script to '$HOME_BIN'."
fi
echo "Done."


# Create a stop script to ensure correct envrionment before killing Raraa process
echo "Creating 'stop' script ... "
export STOP_SOUND="stop_sound.sh"
touch $STOP_SOUND
chmod u+x $STOP_SOUND

echo "#!/bin/bash"                     > $STOP_SOUND
echo "echo \" ... impliment me ... \"" >> $STOP_SOUND

if [ "$DO_LOCAL" = false ]; then
    cp $STOP_SOUND $HOME_BIN
    echo "Copied stop script to '$HOME_BIN'."
fi
echo "Done."


# Create a local area cleanup script to remove local stuff when testing
echo "Creating 'clean_workspace' script ... "
export CLEAN_LOCAL="clean_workspace.sh"
touch $CLEAN_LOCAL
chmod u+x $CLEAN_LOCAL

echo "#!/bin/bash"       > $CLEAN_LOCAL
echo "cd `pwd`/analysis" >> $CLEAN_LOCAL
echo "rm rec*"           >> $CLEAN_LOCAL
echo "cd `pwd`/data"     >> $CLEAN_LOCAL
echo "rm rec*"           >> $CLEAN_LOCAL

echo "Done."



echo " === (4/4) STOP - Prepared raraa === "






echo " === INSTALLATION COMPLETE === "
