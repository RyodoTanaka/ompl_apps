#!/bin/bash

SCRIPT_PATH=$( dirname ${BASH_SOURCE:-$0} )
CURRENT=$(pwd)

if [ "$1" == "clean" ]; then
    echo "Remove bin/ build/ directories..."
    rm -rf ${SCRIPT_PATH}/bin
    rm -rf ${SCRIPT_PATH}/build
else
    if [ -e "$SCRIPT_PATH/build" ]
    then
        echo "build directory exists."
    else
        echo "build directory not found."
        mkdir ${SCRIPT_PATH}/build
    fi

    if [ -e "$SCRIPT_PATH/bin" ]
    then
        echo "bin directory exists."
    else
        echo "bin directory not found."
        mkdir ${SCRIPT_PATH}/bin
    fi

    cd ${SCRIPT_PATH}/build
    cmake .. -DCMAKE_FIND_DEBUG_MODE=1
    make -j`nproc`
    RET=$?
    cd ${CURRENT}
    if [ "$RET" -eq 0 ]; then
        notify-send -i $(pwd)/../../.images/ompl-success.jpg -t 1000 'OMPL ORIG. COMPILER' ' (｀・ω・´) yes!! Original code SUCCEEDED !'
    else
        notify-send -i $(pwd)/../../.images/ompl-fail.jpg -t 1000 'OMPL ORIG. COMPILER' '(´･ω ･`) oops... Original code FAILED...'
    fi
    source setup.bash
    return $RET
fi
