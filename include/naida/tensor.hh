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

    explicit Shape(const std::initializer_list<size_t>& list);

    iterator begin();
    iterator end();

    size_t operator[](int idx) const;
    size_t size() const;
    size_t total_size() const;

    friend std::ostream& operator<<(std::ostream& os, const Shape& shape);
    friend bool operator==(const Shape& lhs, const Shape& rhs);

private:
    std::vector<size_t> vec;
    size_t sz;
};

enum class DType
{
    FLOAT32 = 1
};

size_t dtype_size(const DType dtype);

std::string dtype_str(const DType& dtype);

template<typename T>
std::ostream& print_vector(std::ostream& os, const T* ptr, const Shape& shape, const size_t shape_idx, size_t& offset)
{
    if (shape_idx == shape.size() - 1)
    {
        os << fmt::format("{:.6f}", fmt::join(std::span(ptr + offset, shape[-1]), ","));
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

std::vector<std::byte> random_weights(const Shape& shape, const DType& dtype);

class Tensor
{
public:
    Tensor(const std::vector<std::byte>& buf, const Shape& shape, const DType& dtype = DType::FLOAT32);
    Tensor(const Shape& shape, const DType& dtype = DType::FLOAT32);

    friend std::ostream& operator<<(std::ostream& os, const Tensor& tensor);
    Shape shape() const;
    const std::byte* data() const;

private:
    std::vector<std::byte> buf;
    Shape shape_;
    DType dtype;
};

} // namespace naida