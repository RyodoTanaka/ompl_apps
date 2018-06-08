#!/bin/bash

# install most dependencies via apt-get
sudo apt-get -y update
sudo apt-get -y upgrade
# On Ubuntu 14.04 we need to add a PPA to get a recent compiler (g++-4.8 is too old).
# We also need to specify a Boost version, since the default Boost is too old.
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get -y install gcc-5 g++-5
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 1
#
# We explicitly set the C++ compiler to g++, the default GNU g++ compiler. This is
# needed because we depend on system-installed libraries built with g++ and linked
# against libstdc++. In case `c++` corresponds to `clang++`, code will not build, even
# if we would pass the flag `-stdlib=libstdc++` to `clang++`.
sudo apt-get -y install cmake libboost-all-dev libeigen3-dev libode-dev
export CXX=g++
