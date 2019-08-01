#!/usr/bin/env sh
set -e

PROJECT=demo
TARGET=${PROJECT}.html
BUILD=${BUILD:-Debug}
#VERBOSE="VERBOSE=1"

BUILD_DIR=web-build/${BUILD}
DATA_DIR=../data
OXYGINE_ROOT=../../oxygine-framework
OXYGINE_TOOLS=${OXYGINE_ROOT}/tools

CMAKE="emcmake cmake"
MAKE="emmake make"
PYTHON=python
OXYGINE_WEB_EMBED_FOLDER=${OXYGINE_TOOLS}/others/embed_folder_js.py
BROWSER=chrome
RUN="emrun --browser ${BROWSER}"
BUILD_ATLASSES=build_atlasses.sh

# speed load time by packing our atlasses at compile time
#./optimize.atlasses.png.sh

mkdir -p ${BUILD_DIR}

# copy assets loaded at runtime to build dir
# cp -r ${DATA_DIR}/movies ${BUILD_DIR}
# cp -r ${DATA_DIR}/sounds ${BUILD_DIR}
# cp -r web-assets/* ${BUILD_DIR}

cd ${BUILD_DIR}

#generate cmake project in the "build" folder
${CMAKE} -DCMAKE_BUILD_TYPE=${BUILD} ../..

#build it
${MAKE} ${VERBOSE}
