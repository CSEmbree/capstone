#!/bin/bash

export CIRAINBOW=/home/pi

### SETUP PHASE
export SOUND_BASE_DIR=/home/pi/sounds
SOUND_ERR_TYPE=-1
SOUND_COMPLETE_TYPE=0


if [ -d "$CIRAINBOW" ]; then
    export DATA_DIR="$CIRAINBOW/data/"
else
    export DATA_DIR="$SOUND_BASE_DIR/data/"
fi

echo "TEST: Data dir is: \"${DATA_DIR}\""




### FUNCTIONS
output_msg () {
    TIME=$(date)
    eval NAME="$1"
    eval TYPE="$2"
    eval DATA="$3"

    echo "{"                      >  $NAME
    echo "\"date\": \"${TIME}\"," >> $NAME
    echo "\"type\": ${TYPE},"     >> $NAME
    echo "\"data\": \"${DATA}\""  >> $NAME
    echo "}"                      >> $NAME
}

error_msg () {
    FNAME="$DATA_DIR/sound_install_error-$RANDOM.dat"
    output_msg ${FNAME} ${1} ${2}
}

complete_msg () {
    FNAME="$DATA_DIR/sound_install_complete-$RANDOM.dat"
    output_msg ${FNAME} ${1} ${2}
}


### TEST PHASE

# TEST - error 
MSG_TYPE=$SOUND_ERR_TYPE
MSG_DATA="ERROR: What just happened?!"
error_msg \$MSG_TYPE \$MSG_DATA


# TEST - finish
MSG_TYPE=$SOUND_COMPLETE_TYPE
MSG_DATA="SOUND INSTALLATION SCRIPT COMPLETE."
complete_msg \$MSG_TYPE \$MSG_DATA


