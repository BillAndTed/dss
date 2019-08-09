#!/usr/bin/env sh
set -e

SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
TARGET=demo
BUILD=${BUILD:-Debug}
BUILD_DIR=build/${BUILD}

#tools
VALGRIND=valgrind
KCACHEGRIND=kcachegrind
CALLGRIND_OUTPUT=${SCRIPTPATH}/linux/callgrind.out
VALGRIND_OPTIONS="--tool=callgrind"
# NOTE: the following option puts the callgrind output in our linux folder, BUT prevents the PID from being appended
# I'm thinking writing this to a known location and automatically opening up the output in kcachegrind is better
# than having PID info in the filename. Might want to revisit this.
VALGRIND_OPTIONS="${VALGRIND_OPTIONS} --callgrind-out-file=${CALLGRIND_OUTPUT}"

echo "Running with valgrind options: ${VALGRIND_OPTIONS}"

# Bail with error message if we don't have valgrind installed.
which ${VALGRIND} || echo "Could not find ${VALGRIND} on this system. Is it installed?"

#move to working data folder with resources
cd ../data

#run executable
${VALGRIND} ${VALGRIND_OPTIONS} ./../proj.cmake/${BUILD_DIR}/${TARGET}


which ${KCACHEGRIND} || echo "Could not find ${KCACHEGRIND} on this system. Is it installed?"
${KCACHEGRIND} ${CALLGRIND_OUTPUT}
