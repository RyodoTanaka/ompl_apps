#!/bin/bash

CURRENT=`pwd`

if [ "$1" == "clean" ]; then
    echo "Rebuild the OMPL Library..."
    rm -rf ../../install/ompl/build/Release/*
    cd ../../install/ompl/build/Release
    cmake ../..
    cd $CURRENT
    sudo make -j`nproc` -C ../../install/ompl/build/Release/ install
    if [ "$?" -eq 0 ]; then
        shift
        notify-send -i $(pwd)/../../.images/ompl-success.jpg -t 1000 'OMPL LIB. COMPILER' ' (｀・ω・´) yes!! OMPL Lib COMPILED!'
    else
        notify-send -i $(pwd)/../../.images/ompl-fail.jpg -t 1000 'OMPL LIB. COMPILER' '(´･ω ･`) oops... OMPL Lib NOT COMPILED...'
        return $?
    fi
else
    sudo make -j`nproc` -C ../../install/ompl/build/Release/ uninstall
    sudo make -j`nproc` -C ../../install/ompl/build/Release/ install
fi

if [ "$?" -eq 0 ]; then
    notify-send -i $(pwd)/../../.images/ompl-success.jpg -t 1000 'OMPL LIB. COMPILER' ' (｀・ω・´) yes!! OMPL Lib INTALLED!'
    source .build.bash clean
    source .build.bash
else
    notify-send -i $(pwd)/../../.images/ompl-fail.jpg -t 1000 'OMPL LIB. COMPILER' '(´･ω ･`) oops... OMPL Lib NOT INSTALLED...' 
fi
