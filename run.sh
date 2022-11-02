#!/bin/bash

PROJECT_DIR=$(dirname "$0") #note: this is _not_ the same as used in `make` below: that's the Makefile's own
cd $PROJECT_DIR

[ -z "$BELA_HOME" ] && BELA_HOME=$(realpath "`pwd`/../../")
[ -z "$PROJECT_NAME" ] && PROJECT_NAME=$(basename "`pwd`")

set -e
# note: RUN_FILE= ensures this same file doesn't get called recursively
make -C $BELA_HOME CPPFLAGS="-I\$(PROJECT_DIR)/rnbo/common -I\$(PROJECT_DIR)/rnbo -DRNBO_USE_FLOAT32" PROJECT="$PROJECT_NAME" RUN_FILE=aaaaaaaaaaaaaaaaa $@ $([ 0 -eq "$#" ] && echo run)
