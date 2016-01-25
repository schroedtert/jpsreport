#!/usr/bin/env bash

if [ "$TRAVIS_OS_NAME" == "linux" ]
then
    if [ "$CXX" = "g++" ];
    then
        sudo apt-get install -qq g++-4.8
        export CXX="g++-4.8"
    elif [ "$CXX" == "clang++" ]
    then
        sudo apt-get install --allow-unauthenticated -qq clang-3.4
        export CXX="clang++-3.4";
    fi
elif [ "$TRAVIS_OS_NAME" == "osx" ]
then
    echo "not yet implemented"
fi
