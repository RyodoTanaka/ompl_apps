# OMPL APPs Repository
This Repository is for practice about the OMPL.  
On `CPP/NAIST` directory, there are samples that texted on the [NAIST Online Text](https://robotics.naist.jp/edu/text/?Robotics%2FOMPL).

#### Environment

|       |       |
| :---: | :---: |
| **PC** | Lenovo ThinkPad X240 |
| **Prosessor** |  Intel Core i7-4600U (2.10GHz, 4MB, 1600MHz) |
| **RAM** | PC3-12800 DDR3L (8GB) |
| **OS** | Ubuntu 14.04 LTS 64bit (Trusty) |
| **Kernel** | 3.13.0-108-generic |

## Installation
### Installation of OMPL with shell script
**!! If you want to compile with NAIST samples, BUILD 1.0.0 VERSION !!**  
Based on the [official installation shell script](http://ompl.kavrakilab.org/installation.html), I modified something that is not worked.  
If you've not installed the OMPL library yet, run the shell script like following. And this script bring you OMPL-1.3.0 version.

```bash
$ git clone https://github.com/RyodoTanaka/ompl_apps.git
$ cd ompl_apps
$ chmod u+x ./install/install-ompl-1.3.0-ubuntu.sh
```

And then, You can choose the option as you wish.

- `$ ./install/install-ompl-1.3.0-ubuntu.sh` will install OMPL without Python bindings
- `$ ./install/install-ompl-1.3.0-ubuntu.sh --python` will install OMPL with Python bindings
- `$ ./install/install-ompl-1.3.0-ubuntu.sh --app` will install OMPL.app with Python bindings

#### !!! CAUTION !!!
If you choose `--python` or `--app` option, OMPL will be installed with Python bindings.  
BUT, It takes **ABOUT 8 HOURS** or more....
This is issued on [here](https://bitbucket.org/ompl/ompl/issues/305/omplapp-installation-stuck-at-geometric) and [here](https://bitbucket.org/ompl/ompl/issues/258/update-bindings-after-reinstalling-ompl).

## Installation for developer in C++
#### 1. Go to `cpp/example` directory.
#### 2. Just execute `compile.bash` script.
#### 3. If you add your own program...
Just add configuration into `CMakeLists.txt`.  
And go to 2. procedure.

## How to execute ?
### CPP programs
First of all, you need to compile the programs.
```bash
$ cd <ompl_apps>
$ cd <CPP directory>
$ source build.sh
```
And then run `setup.bash`
```bash
$ source setup.bash
```
This `setup.bash` brings you `omplrun` command.  
`omplrun` command automatically detect the execution files using **Tab Completion**.

### Python programs
Run each python programs in usual way !
