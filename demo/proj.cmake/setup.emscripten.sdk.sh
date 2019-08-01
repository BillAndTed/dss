#!/bin/bash
set -e

WGET=wget
EMSDK_INSTALL_DIR=~/code
EMSDK_PORTABLE=emsdk-portable.tar.gz
EMSDK_PORTABLE_URL=https://s3.amazonaws.com/mozilla-games/emscripten/releases/emsdk-portable.tar.gz

# assuming debian based linux
# we need some packages
#${INSTALL} python2.7
# Install CMake (optional, only needed for tests and building Binaryen)
#${INSTALL} cmake
# Install Java (optional, only needed for Closure Compiler minification)
#${INSTALL} default-jre

# download and install the emscripten 'emsdk'
pushd ${EMSDK_INSTALL_DIR}
if [ ! -f ${EMSDK_PORTABLE} ]; then
    ${WGET} ${EMSDK_PORTABLE_URL}
fi

tar -xvf ${EMSDK_PORTABLE}
cd emsdk-portable


# Fetch the latest registry of available tools.
./emsdk update

# Download and install the latest SDK tools.
./emsdk install latest

# Make the "latest" SDK "active"
./emsdk activate latest

source ./emsdk_env.sh

# TODO: probably best to add installed emsdk dir tools to PATH
# TODO: add 'emsdk activate' to .bashrc
