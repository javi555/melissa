#!/bin/bash

sudo apt --assume-yes install make
sudo apt --assume-yes install cmake
sudo apt --assume-yes install build-essential
sudo apt --assume-yes install dstat
sudo apt update

mkdir -p ~/thirdparties
cd ~/thirdparties/

git clone https://github.com/jbeder/yaml-cpp
cd yaml-cpp/
mkdir build
cd build
cmake -DYAML_BUILD_SHARED_LIBS=ON ..
make
sudo make install

# install ZeroMQ core engine in C++
cd ~/thirdparties/
git clone https://github.com/zeromq/libzmq.git --branch latest_release
cd libzmq/
mkdir build
cd build
cmake ..
make
make test
sudo make install

# install ZeroMQ for C++
cd ~/thirdparties/
git clone https://github.com/zeromq/cppzmq.git
cd cppzmq/
git checkout v4.5.0
mkdir build
cd build
cmake ..
make
make test
sudo make install

# install ZeroMQ for C
cd ~/thirdparties/
git clone git://github.com/zeromq/czmq.git
cd czmq
mkdir build
cd build
cmake ..
sudo make install

# install googletest
cd ~/thirdparties/
wget https://github.com/google/googletest/archive/release-1.10.0.zip
unzip release-1.10.0.zip
rm release-1.10.0.zip
mv googletest-release-1.10.0/ googletest

# install openCV
wget https://github.com/opencv/opencv/archive/3.4.1.tar.gz
tar zxvf 3.4.1.tar.gz
cd opencv-3.4.1
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE ..
make -j4
sudo make install

# install klepsydra
rm -rf ~/thirdparties/klepsydra2
mkdir -p ~/thirdparties/klepsydra2
cd ~/thirdparties/klepsydra2
git clone https://github.com/klepsydra-technologies/kpsr-core ~/thirdparties/klepsydra2/kpsr-core
cd ~/thirdparties/klepsydra2/kpsr-core
git submodule update --init

cd ~/thirdparties/klepsydra2/
git clone https://github.com/klepsydra-technologies/kpsr-robotics ~/thirdparties/klepsydra2/kpsr-robotics
cd ~/thirdparties/klepsydra2/kpsr-robotics
git submodule update --init

# klepsydra core & robotics compilation
# Execute script --> kpsr_comp.sh

# install boost & valgrind
sudo apt install libboost-all-dev
sudo apt install valgrind

# install open-license-mngr
cd ~/thirdparties/klepsydra2
git clone https://javi_aldazabal@bitbucket.org/kpsr-devs/open-license-manager.git
cd open-license-manager/
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../install
make
make install
cd ..
cd example
cmake .
make

# generate license procedure
javi@ubuntu:~/thirdparties/klepsydra2/open-license-manager/example$ ./example 
license ERROR :
    license file not found 
the pc signature is :
    AA0/-QNEe-CcI5-H9Ee
javi@ubuntu:~/thirdparties/klepsydra2/open-license-manager/install/bin$ ./license_generator -s AA0/-QNEe-CcI5-H9Ee -o klepsydra.lic -e 20221231 klepsydra
javi@ubuntu:~/thirdparties/klepsydra2/open-license-manager/install/bin$ ls
klepsydra.lic  license_generator

# install kpsr-admin
cd ~/thirdparties/klepsydra2
git clone https://javi_aldazabal@bitbucket.org/kpsr-devs/kpsr-admin.git
cd kpsr-admin
mkdir build
cd build
cmake -DKPSR_WITH_SOCKET=true ..
make
make test
sudo make install

#install kpsr-openmct
cd ~/thirdparties/klepsydra2
git clone https://javi_aldazabal@bitbucket.org/kpsr-devs/kpsr-openmct.git
cd kpsr-openmct
npm install

#install nodeJS & npm
sudo apt --assume-yes install libssl-dev
sudo apt --assume-yes install libcurl4-openssl-dev
sudo apt --assume-yes install curl
curl -sL https://deb.nodesource.com/setup_11.x | sudo -E bash -
sudo apt-get install -y nodejs
