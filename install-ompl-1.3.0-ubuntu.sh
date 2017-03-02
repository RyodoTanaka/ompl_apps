#!/bin/bash

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
    export MAKEFLAGS="-j`nproc`"
}

install_python_binding_dependencies()
{
    sudo apt-get -y install python-dev python-pip
    # install additional python dependencies via pip
    sudo -H pip install -vU pygccxml https://bitbucket.org/ompl/pyplusplus/get/1.7.0.tar.gz
    # install castxml
    if [ -z $TRUSTY ]; then
        sudo apt-get -y install castxml
    else
        wget -O - https://midas3.kitware.com/midas/download/item/318227/castxml-linux.tar.gz | tar zxf - -C $HOME
        export PATH=$HOME/castxml/bin:$PATH
    fi
}

install_app_dependencies()
{
    # We prefer PyQt5, but PyQt4 also still works.
    if [ -z $TRUSTY ]; then
        sudo apt-get -y install python-pyqt5.qtopengl
    else
        sudo apt-get -y install python-qt4-dev python-qt4-gl
    fi
    sudo apt-get -y install freeglut3-dev libassimp-dev python-opengl python-flask python-celery
    # install additional python dependencies via pip
    sudo -H pip install -vU PyOpenGL-accelerate
    # install libccd
    if [ -z $TRUSTY ]; then
        sudo apt-get -y install libccd-dev
    else
        wget -O - https://github.com/danfis/libccd/archive/v2.0.tar.gz | tar zxf -
        cd libccd-2.0; cmake .; sudo -E make $MAKEFLAGS  install;
		export PKG_CONFIG_PATH=`pwd`:$PKG_CONFIG_PATH
		cd ..
    fi
    # install fcl
    wget -O - https://github.com/flexible-collision-library/fcl/archive/0.5.0.tar.gz | tar zxf -
    cd fcl-0.5.0; cmake .; sudo -E make $MAKEFLAGS install;
	export PKG_CONFIG_PATH=`pwd`:$PKG_CONFIG_PATH
	cd ..
}

install_ompl()
{
    if [ -z $2 ]; then
        OMPL="ompl"
    else
        OMPL="omplapp"
    fi
    wget -O - https://bitbucket.org/ompl/ompl/downloads/$OMPL-1.3.0-Source.tar.gz | tar zxf -
    cd $OMPL-1.3.0-Source
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


if [[ ! -z $PYTHON && ! -z $TRUSTY && `uname -m` = "i386" ]]; then
    echo "There is no pre-built binary of CastXML available for 32-bit Ubuntu 14.04"
    echo "To generate the Python bindings, you first need to compile CastXML from source."
    echo "Alternatively, you could change your OS to either a newer version of Ubuntu or 64-bit Ubuntu 14.04."
    exit 1
fi

install_common_dependencies
if [ ! -z $PYTHON ]; then
    install_python_binding_dependencies
fi
if [ ! -z $APP ]; then
    install_app_dependencies
fi
install_ompl $PYTHON $APP
