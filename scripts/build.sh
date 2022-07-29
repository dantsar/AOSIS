#!/bin/bash

# not the best approach for now..o.
# go into every directory and run make

set -e
. ./scripts/config.sh

mkdir -p $DIR_BUILD

for PROJECTS in $SYSTEM_PROJECTS;
do
    (make --directory=$PROJECTS)
done
