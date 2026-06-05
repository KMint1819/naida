FROM nvidia/cuda:12.9.0-cudnn-devel-ubuntu24.04
WORKDIR /workspace

RUN apt update && apt install -y \
    curl \
    python3.12 \
    python3.12-venv \
    python-is-python3 \
    git \
    cmake \
    build-essential \
    vim \
    gdb \
    clangd \
    clang-format

RUN apt install -y \
    libicu-dev \
    libboost-regex-dev
