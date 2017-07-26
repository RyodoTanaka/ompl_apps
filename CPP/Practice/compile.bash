#!/bin/bash

sudo make -j4 -C ../../install/ompl-1.0.0-Source/build/Release/ uninstall
sudo make -j4 -C ../../install/ompl-1.0.0-Source/build/Release/ install

if [ "$?" -eq 0 ]; then
    notify-send -i $(pwd)/../../.images/ompl-success.jpg -t 1000 'OMPL LIB. COMPILER' ' (｀・ω・´) yes!! OMPL-1.0.0 code SUCCEEDED!'
    source build.bash clean
    source build.bash
    if [ "$?" -eq 0 ]; then
        notify-send -i $(pwd)/../../.images/ompl-success.jpg -t 1000 'OMPL ORIG. COMPILER' ' (｀・ω・´) yes!! Original code SUCCEEDED !'
    else
        notify-send -i $(pwd)/../../.images/ompl-fail.jpg -t 1000 'OMPL ORIG. COMPILER' '(´･ω ･`) oops... Original code FAILED...'
    fi
    source setup.bash
else
    notify-send -i $(pwd)/../../.images/ompl-fail.jpg -t 1000 'OMPL LIB. COMPILER' '(´･ω ･`) oops... OMPL-1.0.0 code FAILED...' 
fi
