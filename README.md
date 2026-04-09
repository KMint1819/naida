# naida
> No-AI CUDA

## Start
```
cd naida

docker build . -t naida
docker run --rm -it --gpus all -v $(pwd):/workspace naida bash
```

- weights loader
- model structure


## Main goals
- specdecode
- cuda kernels
- quantization
- multidevice
- moe

## Improvements
- cuda graph

