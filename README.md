# naida
> No-AI CUDA

## Start
```
cd naida

docker build . -t naida
docker run --rm -it --gpus all -v $(pwd):/workspace naida bash
```

- weights loader
- tests
