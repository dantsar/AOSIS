#!/bin/bash

set -e
. ./scripts/config.sh

mkdir -p $DIR_BUILD

make --directory=kernel rust
