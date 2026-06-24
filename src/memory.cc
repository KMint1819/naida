#include "fmt/format.h"
#include "naida/log.hh"
#include <stdexcept>
#include "naida/memory.hh"

#define CUDA_CALL(err)                         \
    do                                         \
    {                                          \
        cuda_check((err), __FILE__, __LINE__); \
    } while (false)

inline void cuda_check(cudaError_t error_code, const char* file, int line)
{
    if (error_code != cudaSuccess)
    {
        throw std::runtime_error(fmt::format("CUDA Error {}: {}. In file '{}' on line {}\n",
                                             fmt::underlying(error_code), cudaGetErrorString(error_code), file, line));
    }
}

namespace naida
{
std::string format_as(const Silicon sil)
{
    if (sil == Silicon::CPU)
        return "CPU";
    if (sil == Silicon::GPU)
        return "GPU";
    return "";
}

BlobDeleter::BlobDeleter(const Silicon silicon): silicon(silicon) {}
void BlobDeleter::operator()(std::byte* ptr) const
{
    if (ptr == nullptr)
        return;

    if (silicon == Silicon::CPU)
        cpu::free(ptr);
    else if (silicon == Silicon::GPU)
        gpu::free(ptr);
}

std::unique_ptr<Blob> Blob::adopt(std::byte* ptr, const uint64_t sz, const Silicon silicon)
{
    auto deleter = silicon == Silicon::CPU ? cpu::free : gpu::free;

    Storage storage;
    if (silicon == Silicon::CPU)
    {
        storage = { ptr, cpu::free };
    }
    else if (silicon == Silicon::GPU)
    {
        storage = { ptr, gpu::free };
    }
    return std::make_unique<Blob>(std::move(storage), sz, silicon);
}

Blob::Blob(Storage storage, const uint64_t sz, const Silicon silicon)
    : storage(std::move(storage)), size_(sz), silicon_(silicon)
{
    NAIDA_TRACE("Constructed blob on {} with size {}", silicon, sz);
}

Blob::Blob(const Blob& rhs): Blob(rhs.size_, rhs.silicon_)
{
    NAIDA_TRACE("Copying blob...");
    cudaMemcpyKind kind = silicon_ == Silicon::CPU ? cudaMemcpyHostToHost : cudaMemcpyDeviceToDevice;
    CUDA_CALL(cudaMemcpy(storage.get(), rhs.storage.get(), rhs.size_, kind));
}

Blob& Blob::operator=(const Blob& rhs)
{
    NAIDA_TRACE("Assigning blob...");
    size_ = rhs.size_;
    silicon_ = rhs.silicon_;
    cudaMemcpyKind kind = silicon_ == Silicon::CPU ? cudaMemcpyHostToHost : cudaMemcpyDeviceToDevice;
    CUDA_CALL(cudaMemcpy(storage.get(), rhs.storage.get(), rhs.size_, kind));
}

Blob::Blob(const uint64_t sz, const Silicon silicon): size_(sz), silicon_(silicon)
{
    alloc(sz);
    NAIDA_TRACE("Constructed blob on {} with size {}", silicon, sz);
}

void Blob::alloc(const uint64_t sz)
{
    NAIDA_TRACE("Allocating size {} to {}...", sz, silicon_);
    std::byte* ptr = nullptr;
    if (silicon_ == Silicon::CPU)
    {
        cpu::alloc(ptr, sz);
        storage = { ptr, cpu::free };
    }
    else if (silicon_ == Silicon::GPU)
    {
        gpu::alloc(ptr, sz);
        storage = { ptr, gpu::free };
    }
}

void Blob::to(const Silicon other_silicon)
{
    NAIDA_TRACE("Moving blob from {} to {}...", silicon_, other_silicon);
    if (silicon_ == other_silicon)
        return;

    std::byte* new_ptr = nullptr;
    if (other_silicon == Silicon::GPU)
    {
        gpu::alloc(new_ptr, size_);
        CUDA_CALL(cudaMemcpy(new_ptr, storage.get(), size_, cudaMemcpyHostToDevice));
        storage = { new_ptr, gpu::free };
    }
    else if (other_silicon == Silicon::CPU)
    {
        cpu::alloc(new_ptr, size_);
        CUDA_CALL(cudaMemcpy(new_ptr, storage.get(), size_, cudaMemcpyDeviceToHost));
        storage = { new_ptr, cpu::free };
    }
}

uint64_t Blob::size() const
{
    return size_;
}

Silicon Blob::silicon() const
{
    return silicon_;
}
std::byte* Blob::data() const
{
    return storage.get();
}

namespace gpu
{
void alloc(std::byte* ptr, const uint64_t sz)
{
    NAIDA_TRACE("Allocating GPU size {} to ptr {}", sz, fmt::ptr(ptr));
    CUDA_CALL(cudaMalloc(&ptr, sz));
}
void free(std::byte* ptr)
{
    NAIDA_TRACE("Freeing GPU pointer {}", fmt::ptr(ptr));
    CUDA_CALL(cudaFree(ptr));
}
} // namespace gpu

namespace cpu
{
void alloc(std::byte* ptr, const uint64_t sz)
{
    NAIDA_TRACE("Allocating CPU size {} to ptr {}", sz, fmt::ptr(ptr));
    ptr = new std::byte[sz];
}
void free(std::byte* ptr)
{
    NAIDA_TRACE("Freeing CPU pointer {}", fmt::ptr(ptr));
    delete[] ptr;
}
} // namespace cpu
} // namespace naida