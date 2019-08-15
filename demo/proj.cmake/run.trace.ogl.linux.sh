#!/usr/bin/env sh
set -e

SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
TARGET=demo
BUILD=${BUILD:-Debug}
BUILD_DIR=build/${BUILD}

#tools
#apitrace trace --api [gl|egl|d3d7|d3d8|d3d9|dxgi] /path/to/application [args...]
APITRACE=apitrace
QAPITRACE=qapitrace
APITRACE_OUTPUT=${SCRIPTPATH}/linux/demo.trace
APITRACE_OPTIONS="${APITRACE_OPTIONS} trace"
APITRACE_OPTIONS="${APITRACE_OPTIONS} --api gl"
APITRACE_OPTIONS="${APITRACE_OPTIONS} --output=${APITRACE_OUTPUT}"

echo "Running with apitrace options: ${APITRACE_OPTIONS}"

# Bail with error message if we don't have valgrind installed.
which ${APITRACE} || echo "Could not find ${APITRACE} on this system. Is it installed?"

#move to working data folder with resources
cd ../data

#run executable
${APITRACE} ${APITRACE_OPTIONS} ./../proj.cmake/${BUILD_DIR}/${TARGET}


which ${QAPITRACE} || echo "Could not find ${QAPITRACE} on this system. Is it installed?"
${QAPITRACE} ${APITRACE_OUTPUT}
