#!/usr/bin/env sh

OX_TOOLS=../../oxygine-framework/tools
DATA_DIR=../data
INPUT_XML=$1

${OX_TOOLS}/oxyresbuild --debug -x ${INPUT_XML} --src_data ${DATA_DIR} --dest_data ${DATA_DIR}
