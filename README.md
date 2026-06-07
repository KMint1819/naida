# naida (No-AI CUDA)

End to end LLM project that uses as less AI as possible, mostly for educational purposes. No AI agents, no AI searches. Every usage for AI has to be justified in [ai_record.md](./ai_record.md)

> To clarify, this is not to discourage people from using AI. I use AI pretty much 100% of my time now during work, but feel like I'm learning less about the engineering details, which seems to be still worth understanding at least for now.  

## Prerequisite
- `hf`: https://huggingface.co/docs/huggingface_hub/guides/cli

## Start
```
# download gpt2
hf download openai-community/gpt2 model.safetensors

docker build . -t naida
docker run --rm -it --gpus all -v -t $(pwd):/workspace naida bash
```

## Build
```
mkdir build
cmake ..
make -j32
```

## Test
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
