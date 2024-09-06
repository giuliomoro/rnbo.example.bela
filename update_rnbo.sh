#!/bin/bash

[ -z "$1" ] && { echo "Usage: $0 /path/to/rnbo/export" >&2; exit 1; }
set -euo pipefail

RNBO_EXPORT=$1
cd `dirname $0`
# check we are in the correct folder
[ -f render.cpp ]

function extract_number () {
	grep $1 $2| sed "s/.*\([0-9]\+\).*/\1/"
}
VERSION_CMAKE=$RNBO_EXPORT/rnbo/version.cmake
MAJOR=$(extract_number RNBO_VERSION_MAJOR $VERSION_CMAKE)
MINOR=$(extract_number RNBO_VERSION_MINOR $VERSION_CMAKE)
PATCH=$(extract_number RNBO_VERSION_PATCH $VERSION_CMAKE)
VERSION=$MAJOR.$MINOR.$PATCH
echo $VERSION
rm -rf rnbo
cp -r $RNBO_EXPORT/rnbo ./
rm -rf rnbo/RNBO.cpp rnbo/*.json rnbo/adapters rnbo/src/platforms/nostdlib rnbo/test rnbo/cmake
git add rnbo
git commit -m "====Updating rnbo to $VERSION - first step: copy updated rnbo folder" -a
# git apply -3 --whitespace=fix <(git diff 79be4ed..ea9bc8a rnbo) # original patches
git apply -3 --whitespace=fix <(git diff f9d70b5ac5c3aa9988a57203da30cb5a6d8bd5da..672d71580c2907fb9fffa2e37ecc2c232a411d23) # patches used for 1.3.2
git commit -m "====Updating rnbo to $VERSION - second step: apply patches" -a
