FROM rust:latest

RUN set -x \
    && apt-get update \
    && apt-get install -y make gcc-multilib && \
    rustup target add i686-unknown-linux-gnu

VOLUME /code
CMD ["/bin/bash"]
