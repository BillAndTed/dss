#!/bin/bash
set -e

# This script is an aid/document to help install emscripten build tools on your system
# Alternate instructions can be found here: https://emscripten.org/docs/getting_started/downloads.html
# But I've found something like this script is all you need.
# To run, specify an environment variable EMSDK_INSTALL_DIR where you want the emsdk installed,
# and source this script to pick up new paths/env variables
#
# example:
# 'EMSDK_INSTALL_DIR=~/myinstalldir source ./setup.emscripten.sdk.sh'
#
# After running this script you should have access to tools such as 'emcc' and 'em++'
# It's suggested that rather than running this script every time you need those tools, you
# instead source the script ${EMSDK_INSTALL_DIR}/emsdk-portable/emsdk_env.sh in your
# .bashrc or .bash_profile or similar.

WGET=wget
EMSDK_INSTALL_DIR=${EMSDK_INSTALL_DIR:-${HOME}/emscripten}
EMSDK_PORTABLE=emsdk-portable.tar.gz
EMSDK_PORTABLE_URL=https://s3.amazonaws.com/mozilla-games/emscripten/releases/emsdk-portable.tar.gz

# assuming debian based linux
# we need some packages
#${INSTALL} python2.7
# Install CMake (optional, only needed for tests and building Binaryen)
#${INSTALL} cmake
# Install Java (optional, only needed for Closure Compiler minification)
#${INSTALL} default-jre

# download the 'emsdk' if necessary
echo "EMSDK will reside at: ${EMSDK_INSTALL_DIR}"
mkdir -p ${EMSDK_INSTALL_DIR}
pushd ${EMSDK_INSTALL_DIR}
if [ ! -f ${EMSDK_PORTABLE} ]; then
    ${WGET} ${EMSDK_PORTABLE_URL}
fi

# decompress the emsdk if necessary
if [ ! -f emsdk-portable ]; then
    tar -xvf ${EMSDK_PORTABLE}
fi

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

echo "###########################################################################"
echo "Emscripten EMSDK present at ${EMSDK_INSTALL_DIR}"
echo "You should now have access to emscripten tools like emcc, em++, emmake etc."
echo "You may wish to source ${EMSDK_INSTALL_DIR}/emsdk-portable/emsdk_env.sh in your .bashrc or .bash_profile or similar."
echo "###########################################################################"
