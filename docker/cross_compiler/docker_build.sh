#!/bin/bash

# this docker build script was heavily inspired by
# Brett Vickers' MonkOS:
# https://github.com/beevik/MonkOS/tree/master/docker

set -e
. ./scripts/config.sh

uid=$(id -u)
gid=$(id -g)
code=$(realpath .)
docker_image="aosis_build_env"

echo ${code}

# https://stackoverflow.com/a/30543453
if [[ "$(docker images -q ${docker_image} 2> /dev/null)" == "" ]]; then
    docker build -t ${docker_image} "${code}/docker/cross_compiler"
fi

echo "RUNNING CROSS_COMPILER IMAGE"
docker run -it --rm     \
    -v "${code}:/code"  \
    -u ${uid}:${gid}    \
    ${docker_image}  /bin/bash -c "cd /code && make iso"
