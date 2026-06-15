FROM nvidia/cuda:12.9.0-cudnn-devel-ubuntu24.04
WORKDIR /workspace

RUN apt update && apt install -y \
    curl \
    python3.12 \
    python3.12-venv \
    python3-pip \
    python-is-python3 \
    git \
    cmake \
    build-essential \
    vim \
    gdb \
    clang-format

RUN apt install -y \
    libicu-dev \
    libboost-regex-dev

RUN pip instal --break-system-packages \
    torch torchvision --index-url https://download.pytorch.org/whl/cpu