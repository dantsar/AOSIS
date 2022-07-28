FROM rust:1.62.1-slim

RUN set -x \
    && apt-get update \
    && apt-get install make \
    rustup target add i686-unknown-linux-gnu

VOLUME /code
CMD ["/bin/bash"]
