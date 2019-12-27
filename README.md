
Melissa project
======

Melissa offers optimization in the passage of messages between several limited resource nodes and a high-performance node for intensive processing (for example contour detection).

To do this, you will use the ZeroMQ libraries together with Klepsydra, and the results can be seen with the OpenMCT display framework.


# Installation instructions

This section contains installation instructions

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

# Install ZeroMQ

Install ZeroMQ core engine in C++
-----------------------------------
    cd ~/thirdparties/
    git clone https://github.com/zeromq/libzmq.git --branch latest_release
    cd libzmq/
    mkdir build
    cd build
    cmake ..
    make
    make test
    sudo make install

Install ZeroMQ for C++
-------------------------
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


Install ZeroMQ for C
----------------------
    cd ~/thirdparties/
    git clone git://github.com/zeromq/czmq.git
    cd czmq
    mkdir build
    cd build
    cmake ..
    sudo make install

Install googletest
------------------
    cd ~/thirdparties/
    wget https://github.com/google/googletest/archive/release-1.10.0.zip
    unzip release-1.10.0.zip
    rm release-1.10.0.zip
    mv googletest-release-1.10.0/ googletest

Install openCV
--------------
    wget https://github.com/opencv/opencv/archive/3.4.1.tar.gz
    tar zxvf 3.4.1.tar.gz
    cd opencv-3.4.1
    mkdir build
    cd build
    cmake -D CMAKE_BUILD_TYPE=RELEASE ..
    make -j4
    sudo make install

# Install klepsydra

Delete previous klepsydra directory and create new one.

    rm -rf ~/thirdparties/klepsydra
    mkdir -p ~/thirdparties/klepsydra
    cd ~/thirdparties/klepsydra
    git clone https://github.com/klepsydra-technologies/kpsr-core ~/thirdparties/klepsydra/kpsr-core
    cd ~/thirdparties/klepsydra/kpsr-core
    git submodule update --init

    cd ~/thirdparties/klepsydra/
    git clone https://github.com/klepsydra-technologies/kpsr-robotics ~/thirdparties/klepsydra/kpsr-robotics
    cd ~/thirdparties/klepsydra/kpsr-robotics
    git submodule update --init

klepsydra core & robotics compilation
---------------------------------------
Execute "app_info/kpsr_comp.sh" with -DKPSR_WITH_SOCKET=true

Install boost & valgrind
------------------------
    sudo apt install libboost-all-dev
    sudo apt install valgrind

Install kpsr-admin
--------------------
    cd ~/thirdparties/klepsydra2
    git clone https://username@repository-kpsr-admin.git
    cd kpsr-admin
    mkdir build
    cd build
    cmake -DKPSR_WITH_SOCKET=true ..
    make
    make test
    sudo make install

Install kpsr-openmct
---------------------
    cd ~/thirdparties/klepsydra2
    git clone https://username@repository-kpsr-openmct.git
    cd kpsr-openmct
    npm install

Install nodeJS & npm
--------------------
    sudo apt --assume-yes install libssl-dev
    sudo apt --assume-yes install libcurl4-openssl-dev
    sudo apt --assume-yes install curl
    curl -sL https://deb.nodesource.com/setup_11.x | sudo -E bash -
    sudo apt-get install -y nodejs
