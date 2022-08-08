#!/bin/bash

set -e
. ./scripts/config.sh

mkdir -p $DIR_BUILD

echo "in rust.sh"

make --directory=kernel/rust librust.a
