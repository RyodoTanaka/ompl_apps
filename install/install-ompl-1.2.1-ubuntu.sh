#!/bin/bash

# This shell script is based on ompl official install-ompl-ubuntu.sh from
# http://ompl.kavrakilab.org/install-ompl-ubuntu.sh
# I modified about installation of g++-5

mkdir libs
cd libs

if [ `lsb_release -rs` == "14.04" ]; then
    export TRUSTY=1
fi

install_common_dependencies()
{
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
    if [ -z $TRUSTY ]; then
        sudo apt-get -y install cmake libboost-all-dev libeigen3-dev libode-dev
        export CXX=g++
    else
        # needed for the add-apt-repository command, which was not part of early Trusty releases
        sudo apt-get -y install software-properties-common
        sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
        sudo apt-get -y update
        sudo apt-get -y install g++-5 cmake libboost1.55-all-dev libeigen3-dev libode-dev
        export CXX=g++-5
    fi
    #export MAKEFLAGS="-j `nproc`"
    export MAKEFLAGS="-j3"
}

install_python_binding_dependencies()
{
    echo "=== Python Binding Dependencies ==="
    sudo apt-get -y install python-dev python-pip
    # install additional python dependencies via pip
    sudo -H pip install -vU pygccxml https://bitbucket.org/ompl/pyplusplus/get/1.7.0.tar.gz
    #install castxml
    if [ -z $TRUSTY ]; then
        sudo apt-get -y install castxml
    else
        wget -O - https://midas3.kitware.com/midas/download/item/318227/castxml-linux.tar.gz | tar zxf -
        export PATH=`pwd`/castxml/bin:$PATH
    fi
    sudo apt-get install python-p++
}

install_app_dependencies()
{
    # We prefer PyQt5, but PyQt4 also still works.
    sudo apt-get -y install python-pyqt5.qtopengl
    # if [ -z $TRUSTY ]; then
    #     sudo apt-get -y install python-pyqt5.qtopengl
    # else
    #     sudo apt-get -y install python-qt4-dev python-qt4-gl
    # fi
    sudo apt-get -y install freeglut3-dev libassimp-dev python-opengl python-flask python-celery
    # install additional python dependencies via pip
    # sudo -H pip install -vU PyOpenGL-accelerate
    # sudo apt-add-repository -y ppa:imnmfotmal/libccd
    # sudo update
    # sudo apt-get -y install fcl libccd
    # install libccd
    # sudo apt-add-repository -y ppa:libccd-debs/ppa
    # sudo apt-get update
    # sudo apt-get -y install libccd-dev
	export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH
    if [ -z $TRUSTY ]; then
        sudo apt-get -y install libccd-dev
    else
        wget -O - https://github.com/danfis/libccd/archive/v2.0.tar.gz | tar zxf -
        cd libccd-2.0; cmake .; sudo -E make $MAKEFLAGS install; sudo cp -rf ccd.pc /usr/local/lib/pkgconfig/; cd ..
    fi
    #install fcl
    wget -O - https://github.com/flexible-collision-library/fcl/archive/0.5.0.tar.gz | tar zxf -
    cd fcl-0.5.0; cmake .; sudo -E make $MAKEFLAGS install; sudo cp -rf fcl.pc /usr/local/lib/pkgconfig/; cd ..
}

install_ompl()
{
    if [ -z $2 ]; then
        OMPL="ompl"
    else
        OMPL="omplapp"
    fi
    wget -O - https://bitbucket.org/ompl/ompl/downloads/$OMPL-1.3.0-Source.tar.gz | tar zxf -
    cd $OMPL-1.2.1-Source
    mkdir -p build/Release
    cd build/Release
    cmake ../..
    if [ ! -z $1 ]; then
        make $MAKEFLAGS update_bindings
    fi
    make $MAKEFLAGS
    sudo make $MAKEFLAGS install
}

for i in "$@"
do
case $i in
    -a|--app)
        APP=1
        PYTHON=1
        shift
        ;;
    -p|--python)
        PYTHON=1
        shift
        ;;
    *)
        # unknown option -> show help
        echo "Usage: `basename $0` [-p] [-a]"
        echo "  -p: enable Python bindings"
        echo "  -a: enable OMPL.app (implies '-p')"
    ;;
esac
done

install_common_dependencies
if [ ! -z $PYTHON ]; then
    install_python_binding_dependencies
fi
if [ ! -z $APP ]; then
    install_app_dependencies
fi
install_ompl $PYTHON $APP
