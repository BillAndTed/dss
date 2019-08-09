#!/usr/bin/env sh
set -e

SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
TARGET=demo
BUILD=${BUILD:-Debug}
BUILD_DIR=build/${BUILD}

#tools
VALGRIND=valgrind
MASSIF_OUTPUT=${SCRIPTPATH}/linux/massif.out
MASSIF_VISUALIZER=massif-visualizer
VALGRIND_OPTIONS="--tool=massif"
#VALGRIND_OPTIONS="${VALGRIND_OPTIONS} --pages-as-heap=yes"
VALGRIND_OPTIONS="${VALGRIND_OPTIONS} --massif-out-file=${MASSIF_OUTPUT}"

echo "Running with valgrind options: ${VALGRIND_OPTIONS}"

# Bail with error message if we don't have valgrind installed.
which ${VALGRIND} || echo "Could not find ${VALGRIND} on this system. Is it installed?"

#move to working data folder with resources
cd ../data

#run executable
${VALGRIND} ${VALGRIND_OPTIONS} ./../proj.cmake/${BUILD_DIR}/${TARGET}


which ${MASSIF_VISUALIZER} || echo "Could not find ${MASSIF_VISUALIZER} on this system. Is it installed?"
${MASSIF_VISUALIZER} ${MASSIF_OUTPUT}
