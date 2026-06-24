#pragma once

#include <memory>
#include <functional>
#include <cuda_runtime.h>

namespace naida
{

enum class Silicon
{
    CPU = 1,
    GPU = 2
};

std::string format_as(const Silicon sil);

class BlobDeleter
{
public:
    BlobDeleter(const Silicon);
    void operator()(std::byte* ptr) const;

private:
    const Silicon silicon;
};


class Blob
{
    using Storage = std::unique_ptr<std::byte, std::function<void(std::byte*)>>;

public:
    explicit Blob(const Blob&);
    Blob& operator=(const Blob&);
    explicit Blob(const uint64_t sz, const Silicon silicon);
    explicit Blob(Storage storage, const uint64_t sz, const Silicon silicon);
    static std::unique_ptr<Blob> adopt(std::byte*, const uint64_t sz, const Silicon silicon);

    // Alloc a space. Resets the existing pointer
    virtual void alloc(const uint64_t sz);
    virtual void to(const Silicon);
    virtual Silicon silicon() const;
    virtual std::byte* data() const;
    virtual uint64_t size() const;
    virtual ~Blob() {};

private:
    uint64_t size_ = 0;
    Silicon silicon_ = Silicon::CPU;
    Storage storage;
};

namespace gpu
{
void alloc(std::byte* ptr, const uint64_t sz);
void free(std::byte* ptr);
} // namespace gpu

namespace cpu
{
void alloc(std::byte* ptr, const uint64_t sz);
void free(std::byte* ptr);
} // namespace cpu
} // namespace naida