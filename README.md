# naida
> No-AI CUDA

## Prerequisite
- `hf`: https://huggingface.co/docs/huggingface_hub/guides/cli

## Start
```
# download gpt2
hf download openai-community/gpt2 model.safetensors

docker build . -t naida
docker run --rm -it --gpus all -v $(pwd):/workspace naida bash
```

## Build
```
mkdir build
cmake ..
make -j32
```

## Teset
```
cd naida/build

```

- weights loader
- model structure

## Main goals
- specdecode
- cuda kernels
- quantization
- moe
- faster than llama.cpp

## Must-have
- verification per step with torch gpt2

## Improvements
- cuda graph

