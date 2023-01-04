# DOCKER_PROJECTS="rust cross_compiler"
DOCKER_PROJECTS="cross_compiler"


set -e
. ./scripts/config.sh

for PROJECT in $DOCKER_PROJECTS
do
    realpath .
    ./docker/$PROJECT/docker_build.sh
done
