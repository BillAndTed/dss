#!/usr/bin/env sh
set -e

TARGET=demo
BUILD=${BUILD:-Debug}
BUILD_DIR=build/${BUILD}

#move to working data folder with resources
cd ../data

#run executable
./../proj.cmake/${BUILD_DIR}/${TARGET}
