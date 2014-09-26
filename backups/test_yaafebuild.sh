#!/bin/bash



echo " === (2/3) START - Audio feature extraction envrionment setup  === "


YAAFE=`pwd`/yaafe-v0.64

if [ ! -d "$YAAFE" ]; then
    echo "Extracting yaafe files ... "
    tar -xf $YAAFE.tgz
    echo "Done."
fi

cd $YAAFE/
if [ ! -f built ]; then
	echo "Building Yaafe ... "
	ccmake -DCMAKE_INSTALL_PREFIX=. . #install everything to local yaafe
	#ccmake #configure and generate
	sudo make
	sudo make install
	touch built
fi
cd -



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

