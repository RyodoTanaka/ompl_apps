#!/bin/bash

CURRENT=`pwd`

if [ "$1" == "init" ]; then
    echo "Rebuild the OMPL Library..."
    if [ -e ../../install/ompl/build/Release ]; then
        rm -rf ../../install/ompl/build/Release/*
    else
        mkdir -p ../../install/ompl/build/Release
    fi
    cd ../../install/ompl/build/Release
    cmake ../..
    sudo make -j`nproc` install
    if [ "$?" -eq 0 ]; then
        shift
        notify-send -i $(pwd)/../../.images/ompl-success.jpg -t 1000 'OMPL LIB. COMPILER' ' (｀・ω・´) yes!! OMPL Lib COMPILED!'
        cd $CURRENT
    else
        notify-send -i $(pwd)/../../.images/ompl-fail.jpg -t 1000 'OMPL LIB. COMPILER' '(´･ω ･`) oops... OMPL Lib NOT COMPILED...'
        cd $CURRENT
        return $?
    fi
elif [ "$1" == "install" ]; then
    echo "Installation will start."
    sudo make -j`nproc` -C ../../install/ompl/build/Release/ uninstall
    sudo make -j`nproc` -C ../../install/ompl/build/Release/ install
else
    echo "Normal compile will start."
fi

if [ "$?" -eq 0 ]; then
    notify-send -i $(pwd)/../../.images/ompl-success.jpg -t 1000 'OMPL LIB. COMPILER' ' (｀・ω・´) yes!! OMPL Lib INTALLED!'
    source .build.bash clean
    source .build.bash
else
    notify-send -i $(pwd)/../../.images/ompl-fail.jpg -t 1000 'OMPL LIB. COMPILER' '(´･ω ･`) oops... OMPL Lib NOT INSTALLED...' 
fi
