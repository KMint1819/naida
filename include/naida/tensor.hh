#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <ostream>
#include <span>
#include <random>

#include "naida/common.hh"
#include "fmt/ranges.h"

namespace naida
{

class Shape
{
public:
    using iterator = std::vector<size_t>::iterator;
    using const_iterator = std::vector<size_t>::const_iterator;

    Shape(const std::vector<size_t>& vec): vec(vec)
    {
        size_t tmp = vec.size() > 0 ? 1 : 0;

        for (const size_t sz : vec)
            tmp *= sz;
        sz = tmp;
    }
    Shape(const std::initializer_list<size_t>& list): vec(list) {}

    iterator begin()
    { return vec.begin(); }
    iterator end()
    { return vec.end(); }

    size_t operator[](int idx) const
    {
        while (idx < 0)
            idx += vec.size();

        if (idx >= vec.size())
            throw std::range_error("Shape idx out of range");


        return vec[idx];
    }

    size_t size() const
    { return vec.size(); }
    size_t total_size() const
    { return sz; }

    friend std::ostream& operator<<(std::ostream& os, const Shape& shape)
    {
        os << fmt::format("{{}}", shape.vec);
        return os;
    }


private:
    std::vector<size_t> vec;
    size_t sz;
};

enum class DType
{
    FLOAT32 = 1
};

size_t dtype_size(const DType dtype)
{
    switch (dtype)
    {
    case DType::FLOAT32: return 4; break;
    }
    return 0;
}

std::string dtype_str(const DType& dtype)
{
    switch (dtype)
    {
    case DType::FLOAT32: return "Float32"; break;
    default: return "Unknown type";
    }
    return "Unknown type";
}

template<typename T>
std::ostream& print_vector(std::ostream& os, const T* ptr, const Shape& shape, const size_t shape_idx, size_t& offset)
{
    if (shape_idx == shape.size() - 1)
    {
        os << fmt::format("{}", fmt::join(std::span(ptr + offset, shape[-1]), ","));
        offset += shape[-1];
    }
    else
    {
        for (size_t i = 0; i < shape[shape_idx]; i++)
        {
            os << "[";
            print_vector(os, ptr, shape, shape_idx + 1, offset);
            os << "]\n";
        }
    }
    return os;
}

std::vector<std::byte> random_weights(const Shape& shape, const DType& dtype)
{
    std::vector<std::byte> buf;
    buf.reserve(shape.total_size() * dtype_size(dtype));

    static std::random_device dev;
    static std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 255);
    for (int i = 0; i < shape.total_size() * dtype_size(dtype); i++)
    {
        buf.push_back(std::byte { static_cast<unsigned char>(dist(rng)) });
    }
    return buf;
}

class Tensor
{
public:
    Tensor(const std::vector<std::byte>& buf, const Shape& shape, const DType& dtype)
        : buf(buf), shape(shape), dtype(dtype)
    {
    }
    Tensor(const Shape& shape, const DType& dtype): Tensor(random_weights(shape, dtype), shape, dtype) {}

    friend std::ostream& operator<<(std::ostream& os, const Tensor& tensor)
    {
        os << dtype_str(tensor.dtype) << " tensor (" << tensor.shape << ")\n";

        if (tensor.dtype == DType::FLOAT32)
        {
            const float* ptr = reinterpret_cast<float*>(const_cast<std::byte*>(tensor.buf.data()));
            size_t offset = 0;
            print_vector(os, ptr, tensor.shape, 0, offset);
        }
        else
            throw std::runtime_error("Type is not supported");

        return os;
    }


private:
    std::vector<std::byte> buf;
    Shape shape;
    DType dtype;
};

} // namespace naida