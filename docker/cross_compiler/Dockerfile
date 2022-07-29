# TO DO: create docker image of the build environment
FROM randomdude/gcc-cross-i686-elf

RUN set -x \
    && apt-get update \
    && apt-get install -y nasm xorriso grub-common grub-pc-bin

VOLUME /code
CMD ["/bin/bash"]
