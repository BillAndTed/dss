#!/usr/bin/env sh
set -e

BUILD=${BUILD:-Debug}

BUILD_DIR=web-build/${BUILD}
NAME=index
TARGET=${NAME}.html
RUN=emrun

cd ${BUILD_DIR}
${RUN} ${TARGET}

