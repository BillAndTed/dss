#!/usr/bin/env sh
set -e

BUILD=${BUILD:-Debug}

BUILD_DIR=build/${BUILD}

mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

#generate cmake project in the "build" folder
cmake -DCMAKE_BUILD_TYPE=${BUILD} ../..

#build it
make

