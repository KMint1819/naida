#pragma once
#include <vector>
#include <string>
#include <ostream>
#include <span>
#include <memory>

#include "naida/common.hh"
#include "naida/memory.hh"
#include <fmt/ranges.h>

namespace naida
{

class Shape
{
public:
    using iterator = std::vector<size_t>::iterator;
    using const_iterator = std::vector<size_t>::const_iterator;

    template<class It>
    explicit Shape(It start, It end): vec(start, end)
    {
        size_t tmp = vec.size() > 0 ? 1 : 0;

        for (const size_t sz : vec)
            tmp *= sz;
        sz = tmp;
    }
    explicit Shape(const std::initializer_list<size_t>& list);
    explicit Shape(const Shape&) = default;
    explicit Shape(Shape&) = default;

    iterator begin();
    iterator end();

    size_t operator[](int idx) const;

    // Returns size of the shape vector (e.g for shape [2, 3], size is 2)
    size_t size() const;
    // Returns total size of the shape vector (e.g for shape [2, 3], size is 6)
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


class Tensor : public Formattable
{
public:
    static std::unique_ptr<Tensor> zeros(const Shape& shape, const DType dtype = DType::FLOAT32,
                                         const Silicon sil = Silicon::CPU);
    // CPU only
    Tensor(std::unique_ptr<std::byte[]> bytes, const Shape&, const DType dtype = DType::FLOAT32);

    // required for std::copy
    Tensor(const Tensor& rhs);
    Tensor& operator=(const Tensor& rhs);
    Tensor(std::unique_ptr<Blob>, const Shape& shape, const DType dtype = DType::FLOAT32);
    virtual std::string to_string() const override;
    friend std::ostream& operator<<(std::ostream& os, const Tensor& tensor);
    Shape shape() const;
    Silicon silicon() const;
    const std::byte* data() const;

private:
    std::unique_ptr<Blob> blob;
    Shape shape_;
    DType dtype_;
};
} // namespace naida