#!/usr/bin/env sh
set -e

TARGET=demo
BUILD=${BUILD:-Debug}
BUILD_DIR=build/${BUILD}

#tools
export VALGRIND=valgrind
VALGRIND_OPTIONS="--tool=memcheck --leak-check=full"

# Bail with error message if we don't have valgrind installed.
which ${VALGRIND} || echo "Could not find ${VALGRIND} on this system. Is it installed?"

#move to working data folder with resources
cd ../data

#run executable
${VALGRIND} ${VALGRIND_OPTIONS} ./../proj.cmake/${BUILD_DIR}/${TARGET}
