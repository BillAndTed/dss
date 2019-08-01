#!/bin/bash
set -e

BUILD=Release
BUILD_DIR=web-build/${BUILD}
WEB_DIR=~/public_html/dss-demo

mkdir -p ${WEB_DIR}

cp ${BUILD_DIR}/index.html ${WEB_DIR}
cp ${BUILD_DIR}/demo.js ${WEB_DIR}
cp ${BUILD_DIR}/demo.wasm ${WEB_DIR}
cp ${BUILD_DIR}/demo.data ${WEB_DIR}


