#!/bin/bash

[ -z "$1" ] && { echo "Usage: $0 /path/to/rnbo/export" >&2; exit 1; }
set -euo pipefail

RNBO_EXPORT=$1
cd `dirname $0`
# check we are in the correct folder
[ -f render.cpp ]

rm -rf rnbo
cp -r $RNBO_EXPORT/rnbo ./
rm -rf rnbo/RNBO.cpp rnbo/*.json rnbo/adapters rnbo/src/platforms/nostdlib rnbo/test
git add rnbo
git commit -m "====Updating rnbo - first step: copy updated rnbo folder" -a
git apply -3 --whitespace=fix <(git diff 79be4ed..ea9bc8a rnbo)
git commit -m "====Updating rnbo - second step: apply patches" -a
