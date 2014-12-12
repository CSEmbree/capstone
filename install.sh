#!/bin/bash

# README: This installation script attempts to: 
# 1. Identify a recording device
# 2. Update/install dependant libraries for recording audio and audio analysis with Yaafe 
# 3. Compile any libraries needed for audio recording/analysis
# 4. Create start and stop scripts for audio recording/analysis



# dependant libraries that need to be installed or updated if they do not exist
DEPENDANT_LIBS="cmake cmake-curses-gui libargtable2-0 libargtable2-dev libsndfile1 libsndfile1-dev libmpg123-0 libmpg123-dev libfftw3-3 libfftw3-dev liblapack-dev libhdf5-serial-dev libhdf5-7 python flac alsa-utils"

# work flags - what to do state
DO_LOCAL=false
DO_UPDATE=true
DO_COMPILE=true
DO_VERBOSE=false
DO_OUT=true

# error code types
SOUND_ERR_TYPE=-1
SOUND_COMPLETE_TYPE=0

export SOUND_BASE_DIR=`pwd`


# JSON data message formatted helpers to report status 
output_msg () {
    TIME=$(date)
    eval NAME="$1"
    eval TYPE="$2"
    eval DATA="$3"

    echo "{"                          >  $NAME
    echo "    \"date\": \"${TIME}\"," >> $NAME
    echo "    \"type\": ${TYPE},"     >> $NAME
    echo "    \"data\": \"${DATA}\""  >> $NAME
    echo "}"                          >> $NAME
}

# Error message has a default name
error_msg () {
    FNAME="$DATA_DIR/sound_install_error-$RANDOM.dat"
    output_msg ${FNAME} ${1} ${2}
}

# For sending a message when installation has complete
complete_msg () {
    FNAME="$DATA_DIR/sound_install_complete-$RANDOM.dat"
    output_msg ${FNAME} ${1} ${2}
}

# Wrapper for echoing install scripts steps to a file as well
out () {
    if [ "$DO_VERBOSE" = true ] ; then
	echo "$@"
    fi


#    echo "TEST ORIG: $@"
#    echo "$@" | tr '\n' ", "
#    ORIG="$@"
#    REP="\\n"
#    res=${ORIG//"\n"/"\\n"}
#    echo "TEST REP: $res"


    if [ "$DO_OUT" = true ] ; then
	OUTPUT=$@ | tr '\n' ","
	OUTPUT=`echo "$@" | tr '\n' " "`
	
	export REPORT="$REPORT\\n$OUTPUT"
    fi

#    export REPORT="$REPORT\\n$@"
}



# exit gracefully and report that a problem may have occured
function exit_steps {

    # Perform program exit housekeeping
    # Stuff goes here...


    out "TERMINATION ENCOUNTERED!"

    MSG_TYPE=$SOUND_ERR_TYPE
    MSG_DATA="Install script was forced to stop! Install State: $REPORT"

    error_msg \$MSG_TYPE \$MSG_DATA
    exit
}

trap exit_steps SIGHUP SIGINT SIGTERM






out " === (1/4) START - Prepapring envrionment for installation  === "

# Check for OPTIONAL argument parameters 
# argument code from (http://stackoverflow.com/questions/192249/how-do-i-parse-command-line-arguments-in-bash)
for i in "$@"
do
    case $i in
	-l|--local)
	    DO_LOCAL=true
	    shift
	    ;;
	-nl|--nolocal)
	    DO_LOCAL=false
	    shift
	    ;;
	-u|--update)
	    DO_UPDATE=true
	    shift
	    ;;
	-nu|--noupdate)
	    DO_UPDATE=false
	    shift
	    ;;
	-c|--compile)
	    DO_COMPILE=true
	    shift
	    ;;
	-nc|--nocompile)
	    DO_COMPILE=false
	    shift
	    ;;
	-o|--out)
	    DO_OUT=true
	    shift
	    ;;
	-no|--noout)
	    DO_OUT=false
	    shift
	    ;;
	-v|--verbose)
	    DO_VERBOSE=true
	    shift
	    ;;
	*)
    # unknown option
	    out "WARNING: Unrecognized argument '$key' !"
	    ;;
    esac
done




# let user know what install steps are going to happen
out "************************************************"
out "Install script will:"

if [ "$DO_UPDATE" = true ] ; then
    out " -- Update dependant libraries: $DEPENDANT_LIBS"
fi

out " -- Create start and stop scripts locally. "

if [ "$DO_COMPILE" = true ] ; then
    out " -- Build Raraa. "
else 
    out " -- NOT build Raraa anew"
fi

if [ "$DO_LOCAL" = false ] ; then
    out " -- Copy start and stop scripts to '$HOME_BIN'. "
fi
out "************************************************"




# Set the default bin and data directory. "HOME" directory is default for bin. "CIRAINBOW" is default for data.
if [ -d "$CIRAINBOW" ]; then
    export HOME_BIN="$CIRAINBOW/bin/"
    export DATA_DIR="$CIRAINBOW/data/"
    out "CIRAINBOW env var found, bin and data directories are no longer default"
else
    export HOME_BIN="$HOME/bin/"
    export DATA_DIR="$SOUND_BASE_DIR/data/"
fi



# Report the directories that are found and could be used
out "Using sound base directory: ${SOUND_BASE_DIR}"
out "Using bin directory:        ${HOME_BIN}"
out "Using data directory:       ${DATA_DIR}"



out " === (1/4) STOP - Finished preparing environment for installation  === "






out " === (2/4) START - Recording envrionment is being prepared  === "

# Update depedant libraries before compiling anything that might use them (user can turn this off by argument)
if [ "$DO_UPDATE" = true ] ; then
    out "Installing/updating dependant libraries ... "
    
    case "$OSTYPE" in
	darwin*)  
	    # install dependant libs
	    out "OS is OSX"
	    sudo port install $DEPENDANT_LIBS
	    out "Done."

	    # update OS/ports
	    sudo port update
	    out "Done." ;;
	linux*)
	    # install dependant libs
	    out "OS is LINUX"
	    sudo apt-get -y install $DEPENDANT_LIBS
	    out "Done."
	    
	    # update OS
	    out "Updating RPi software ... "
	    sudo apt-get -y update
	    sudo apt-get -y upgrade
	    sudo apt-get -y install rpi-update
	    sudo rpi-update
	    out "Done." ;;	
	*)        
	    out "WARNING: OS '$OSTYPE' UNSUPPORTED. Continuing assuming the following dependant libraries are installed and updated: '"+$DEPENDANT_LIBS+"'." ;;
    esac
else
    out "Skipping update step ... "
fi


# prepare audio environment
out "Adding current user to the audio group ... "
res=`sudo usermod -a -G audio pi`
res=$res | tr "\n" "\\n"
out "$res"
out "Done."

out "Listing visible sound cards ... "
res=`lsusb`
res=$res | tr "\n" "\\n"
out "$res"
out "Done."

out "Setting 'amixer' recording settings ... "
res=`amixer -c 1 cset numid=7,iface=MIXER,name='Auto Gain Control' 0`
res=$res | tr "\n" "\\n"
out "$res"
res=`numid=7,iface=MIXER,name='Auto Gain Control'`
res=$res | tr "\n" "\\n"
out "$res"
out "Done."

out "Storing recording settings ... "
res=`sudo alsactl store 1`
res=$res | tr "\n" "\\n"
out "$res"
out "Done."

out "Checking 'arecord' version ... "
res=`arecord --version`
res=$res | tr "\n" "\\n"
out "$res"
out "Done."

out "Making sure sound card is visible ... "
res=`arecord -l`
res=$res | tr "\n" "\\n"
out "$res"
out "Done."

out " === (2/4) STOP - Recording envrionment is setup  === "






out " === (3/4) START - Audio feature extraction envrionment setup  === "

out "Making sure sound card is visible..."
out " ... impliment me ... "
out "Done."


out "Detecting Yaafe ... "
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
	out "Yaafe directory not found locally! Creating one ... "

	# Look for local tarball
	if [ ! -f $YAAFE.tgz ]; then
	    # The Yaafe tarball was not locally, get it from online
	    out "Yaafe tar ball not found locally! Downloading Yaafe ... "
            wget -O $YAAFE_NAME.tgz https://sourceforge.net/projects/yaafe/files/$YAAFE_NAME.tgz/download
	fi

	# Extract the local Yaafe tarball
	out "Extracting yaafe files ... "
	tar -xf $YAAFE.tgz
	out "Done."
    fi
    out "Done."
    

    # A Yaafe install directory exists locally, check it if it has not been built/compiled already
    out "Building Yaafe ... "
    cd $YAAFE/


    # Build check by placing a 'built' empty file in yaafe after first compilation
    if [ ! -f built ]; then
	out "Yaafe being built for the first time ... "
	cmake -DCMAKE_INSTALL_PREFIX=. . #install everything to local yaafe
        #ccmake #configure and generate
	sudo make
	sudo make install
	touch built # built flag for future reference that we have already built Yaafe
    fi
    cd -
    out "Done."

    #To easily use Yaafe locally, we should set the following environment vars:
    export YAAFE_PATH=$YAAFE/yaafe_extensions
    export PATH=$PATH:$YAAFE/bin
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$YAAFE/lib
    export PYTHONPATH=$PYTHONPATH:$YAAFE/python_packages
else
    # If Yaafe exists in the envrionment paths, we assume all paths are correct and handled by someone else (nice!)
    out "YAAFE found! (YAAFE_PATH = $YAAFE_PATH)."
    YAAFE_EXISTED=true
    out "Assuming: PATH, LD_LIBRARY_PATH are setup for Yaafe. Assuming PYTHONPATH exists."
fi



# Sanity check on environment
out "Important Yaafe paths are as follows ... "
out "YAAFE install directory = $YAAFE"
out "PATH = $PATH"
out "LD_LIBRARY_PATH = $LD_LIBRARY_PATH"
out "PYTHONPATH = $PYTHONPATH"
out "Done."

out " === (3/4) STOP - Audio feature extraction envrionment setup  === "






out " === (4/4) START - Preparing raraa === "

# Prepare to compile Raraa and create start and stop scripts for Raraa
if [ "$DO_COMPILE" = true ]; then
    out "cleaning up old workspace ... "
    #make clean
    out "Done."
    
    out "Preparing Raraa ... "
    res=`make`
    out "$res"
    out "Done."
else
    out "Skipping compile step ... "
fi


# Ensure a /home/bin/ directory exists if user wants start/stop scripts copied there
if [ "$DO_LOCAL" = false ]; then
    out "Checking if a '$HOME_BIN' directory exists for sound start and stop scripts."

    if [ ! -d "$HOME_BIN" ]; then
	mkdir $HOME_BIN
	out "Created '$HOME_BIN' for the first time."
    else
	out "'$HOME_BIN' already exists."
    fi
    
    out "Done."
else
    out "Skipping script coping to global step ... "
fi


# Create a start script to ensure correct envrionment before running Raraa
out "Creating 'start' script ... "
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
echo "echo \$@"                                                   >> $START_SOUND
echo "(cd \$SOUND_BASE_DIR && ./raraa \$@)"                       >> $START_SOUND

if [ "$DO_LOCAL" = false ]; then
    cp $START_SOUND $HOME_BIN
    out "Copied start script to '$HOME_BIN'."
fi
out "Done."


# Create a stop script to ensure correct envrionment before killing Raraa process
out "Creating 'stop' script ... "
export STOP_SOUND="stop_sound.sh"
touch $STOP_SOUND
chmod u+x $STOP_SOUND

echo "#!/bin/bash" > $STOP_SOUND
echo "pkill raraa" >> $STOP_SOUND

if [ "$DO_LOCAL" = false ]; then
    cp $STOP_SOUND $HOME_BIN
    out "Copied stop script to '$HOME_BIN'."
fi
out "Done."


# Create a local area cleanup script to remove local stuff when testing
out "Creating 'clean_workspace' script ... "
export CLEAN_LOCAL="clean_workspace.sh"
touch $CLEAN_LOCAL
chmod u+x $CLEAN_LOCAL

echo "#!/bin/bash"                      > $CLEAN_LOCAL
echo "cd `pwd`/analysis"                >> $CLEAN_LOCAL
echo "rm -r {sound*,rec*} 2> /dev/null" >> $CLEAN_LOCAL
echo "cd `pwd`/data"                    >> $CLEAN_LOCAL
echo "rm -r {sound*,rec*} 2> /dev/null" >> $CLEAN_LOCAL
echo "cd `pwd`/data/media"              >> $CLEAN_LOCAL
echo "rm -r {sound*,rec*} 2> /dev/null" >> $CLEAN_LOCAL


out "Done."



out " === (4/4) STOP - Prepared raraa === "




out " === INSTALLATION COMPLETE === "




# Report to the user that the install script has finished
if [ "$DO_OUT" = true ] ; then
    MSG_TYPE=$SOUND_COMPLETE_TYPE
    MSG_DATA="SOUND INSTALLATION SCRIPT COMPLETE. "
    
    MSG_DATA="$MSG_DATA The following was the output: $REPORT"

    complete_msg \$MSG_TYPE \$MSG_DATA
fi


./start_sound.sh