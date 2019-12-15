#!/bin/bash

KPSR_INSTALL_PATH=~/thirdparties/klepsydra2

IFS=","

echo "Installing klepsydra projects"

echo "Removing older installed versions..."
sudo rm -rf /usr/local/include/klepsydra
sudo rm -rf /usr/local/include/kpsr*
sudo rm -rf /usr/loca/lib/cmake/klepsydra
sudo rm -rf /usr/local/lib/cmake/kpsr*

while read proj repo doCmake doMake doInstall doTest; do
    echo "ðŸš€ Installing ${repo}"
    cd ${KPSR_INSTALL_PATH}/$repo
    pwd

    if [ "$doCmake" == "true" ]; then
        sudo rm -rf build
        mkdir build
        cd build
        echo "PWD: $PWD"
        echo "CMAKE"
        cmake -DKPSR_WITH_ZMQ=true -DKPSR_WITH_NODE=true -DKPSR_WITH_YAML=true ..

        if [ "$doMake" == "true" ]; then
            echo "MAKE"
            make -j4

            if [ "$doTest" == "true" ]; then
                echo "TEST"
                make test
            fi

            if [ "$doInstall" == "true" ]; then
                echo "INSTALL"
                sudo make install
            fi
        fi
    fi
done </home/${USER}/projects/melissa/modules/app_info/klepsydra_projects.csv


exit
