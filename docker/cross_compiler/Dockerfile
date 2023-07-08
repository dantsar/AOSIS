# TO DO: create docker image of the build environment
FROM randomdude/gcc-cross-i686-elf

RUN set -x \
    && echo "deb http://archive.debian.org/debian stretch main contrib non-free" > /etc/apt/sources.list \
    && apt-get update \
    && apt-get install -y nasm xorriso grub-common grub-pc-bin

VOLUME /code
CMD ["/bin/bash"]
