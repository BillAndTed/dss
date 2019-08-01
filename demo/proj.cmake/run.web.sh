#!/usr/bin/env sh
set -e

NAME=demo
TARGET=${NAME}.html
RUN=emrun
BUILD_DIR=web

cd ${BUILD_DIR}
${RUN} ${TARGET}

