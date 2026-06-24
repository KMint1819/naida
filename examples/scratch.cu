#include <memory>
#include <cuda_runtime.h>
#include <cstdio>

__global__ void kernel(int* a)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    a[x] = a[x] + 1;
}

int main()
{
    auto a = std::make_unique<std::byte[]>(5);
    for (int i = 0; i < 5; i++)
    {
        printf("%d\n", a[i]);
    }

    kernel<<<1, 5>>>(reinterpret_cast<int*>(a.get()));
    cudaDeviceSynchronize();
    return 0;
}