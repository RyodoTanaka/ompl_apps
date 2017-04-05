#!/bin/bash

sudo make -j4 -C ../../install/ompl-1.0.0-Source/build/Release/ install
source build.bash clean
source build.bash
source setup.bash
