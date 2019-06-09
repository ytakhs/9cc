FROM ubuntu:18.10

RUN groupadd --gid 2222 ubuntu && \
    useradd -m ubuntu --home /home/ubuntu --shell /bin/bash --uid 3333 --gid 2222 && \
    apt update && \
    apt install -y gcc make git binutils libc6-dev && \
    apt clean

USER ubuntu

WORKDIR /home/ubuntu
