#include "naida/tensor.hh"
#include "naida/memory.hh"
#include <cstddef>
#include <initializer_list>
#include <memory>


namespace naida
{
namespace
{
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
std::vector<std::byte> random_weights(const Shape& shape, const DType& dtype)
{
    std::vector<std::byte> buf;
    buf.reserve(shape.total_size() * dtype_size(dtype));

    static std::random_device dev;
    static std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 256);
    for (int i = 0; i < shape.total_size() * dtype_size(dtype); i++)
    {
        buf.push_back(std::byte { static_cast<unsigned char>(dist(rng)) });
    }
    return buf;
}
} // namespace
Shape::Shape(const std::initializer_list<size_t>& list): Shape(list.begin(), list.end()) {}

Shape::iterator Shape::begin()
{
    return vec.begin();
}
Shape::iterator Shape::end()
{
    return vec.end();
}

size_t Shape::operator[](int idx) const
{
    while (idx < 0)
        idx += vec.size();

    if (idx >= vec.size())
        throw std::range_error("Shape idx out of range");


    return vec[idx];
}

size_t Shape::size() const
{
    return vec.size();
}
size_t Shape::total_size() const
{
    return sz;
}

std::ostream& operator<<(std::ostream& os, const Shape& shape)
{
    os << fmt::format("{}", shape.vec);
    return os;
}

std::unique_ptr<Tensor> Tensor::zeros(const Shape& shape, const DType dtype, const Silicon silicon)
{
    auto blob = std::make_unique<Blob>(dtype_size(dtype) * shape.total_size(), silicon);
    return std::make_unique<Tensor>(std::move(blob), shape, dtype);
}

Tensor& Tensor::operator=(const Tensor& rhs)
{
    shape_ = rhs.shape_;
    dtype_ = rhs.dtype_;
    blob = std::unique_ptr<Blob>(new Blob(*rhs.blob));
}

Tensor::Tensor(const Tensor& rhs): shape_(rhs.shape_), dtype_(rhs.dtype_)
{
    blob = std::unique_ptr<Blob>(new Blob(*rhs.blob));
}
Tensor::Tensor(std::unique_ptr<Blob> new_blob, const Shape& shape, const DType dtype)
    : shape_(shape), blob(std::move(new_blob)), dtype_(dtype)
{
}

std::string Tensor::to_string() const
{
    std::stringstream ss;
    ss << dtype_str(dtype_) << " tensor (" << shape_ << ")\n";

    if (dtype_ == DType::FLOAT32)
    {
        const float* ptr = reinterpret_cast<float*>(const_cast<std::byte*>(blob->data()));
        size_t offset = 0;
        print_vector(ss, ptr, shape_, 0, offset);
    }
    else
        throw std::runtime_error("Type is not supported");

    return ss.str();
}
bool operator==(const Shape& lhs, const Shape& rhs)
{
    if (lhs.size() != rhs.size())
        return false;
    if (lhs.total_size() != rhs.total_size())
        return false;

    for (int i = 0; i < lhs.size(); i++)
        if (lhs[i] != rhs[i])
            return false;
    return true;
}
Shape Tensor::shape() const
{
    return Shape { shape_ };
};
const std::byte* Tensor::data() const
{
    return blob->data();
}
Silicon Tensor::silicon() const
{
    return blob->silicon();
}
} // namespace naida