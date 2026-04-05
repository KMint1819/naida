
FROM nvidia/cuda:12.9.0-cudnn-devel-ubuntu24.04
WORKDIR /workspace

RUN curl -LsSf https://hf.co/cli/install.sh | bash
RUN hf download openai-community/gpt2 model.safetensors

