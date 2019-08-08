#!/usr/bin/env sh
set -e

SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
TARGET=demo
BUILD=${BUILD:-Debug}
BUILD_DIR=build/${BUILD}

#tools
VALGRIND=valgrind
VALGRIND_OPTIONS="--tool=memcheck"

# For full verbose leak checks run with option:
VALGRIND_OPTIONS="${VALGRIND_OPTIONS} --leak-check=full"

# To run with valgrind false positives turned off (usually X11 related on linux)
# run with option:
VALGRIND_OPTIONS="${VALGRIND_OPTIONS} --suppressions=${SCRIPTPATH}/linux/valgrind.suppressions"

# To generate a suppression file (which can quieten valgrind false positives)
# run with option:
#VALGRIND_OPTIONS="${VALGRIND_OPTIONS} --gen-suppressions=all"

echo "Running with valgrind options: ${VALGRIND_OPTIONS}"

# Bail with error message if we don't have valgrind installed.
which ${VALGRIND} || echo "Could not find ${VALGRIND} on this system. Is it installed?"

#move to working data folder with resources
cd ../data

#run executable
${VALGRIND} ${VALGRIND_OPTIONS} ./../proj.cmake/${BUILD_DIR}/${TARGET}
