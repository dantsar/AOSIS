#!/bin/bash

set -e
. ./scripts/config.sh

for PROJECTS in $SYSTEM_PROJECTS; 
do
    cd $PROJECTS && make clean
done

rm -rf build *.iso
