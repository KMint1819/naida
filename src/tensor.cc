#include "naida/tensor.hh"

namespace naida
{

Shape::Shape(const std::initializer_list<size_t>& list): vec(list)
{
    size_t tmp = vec.size() > 0 ? 1 : 0;

    for (const size_t sz : vec)
        tmp *= sz;
    sz = tmp;
}

Shape::iterator Shape::begin()
{ return vec.begin(); }
Shape::iterator Shape::end()
{ return vec.end(); }

size_t Shape::operator[](int idx) const
{
    while (idx < 0)
        idx += vec.size();

    if (idx >= vec.size())
        throw std::range_error("Shape idx out of range");


    return vec[idx];
}

size_t Shape::size() const
{ return vec.size(); }
size_t Shape::total_size() const
{ return sz; }

std::ostream& operator<<(std::ostream& os, const Shape& shape)
{
    fmt::print("shape vec size: {}\n", shape.vec.size());
    fmt::print("shape vec: {}\n", shape.vec);
    os << fmt::format("{}", shape.vec);
    return os;
}


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
    printf("%d\n", shape.total_size() * dtype_size(dtype));
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

Tensor::Tensor(const std::vector<std::byte>& buf, const Shape& shape, const DType& dtype)
    : buf(buf), shape_(shape), dtype(dtype)
{
}
Tensor::Tensor(const Shape& shape, const DType& dtype): Tensor(random_weights(shape, dtype), shape, dtype) {}

std::ostream& operator<<(std::ostream& os, const Tensor& tensor)
{
    os << dtype_str(tensor.dtype) << " tensor (" << tensor.shape_ << ")\n";

    if (tensor.dtype == DType::FLOAT32)
    {
        const float* ptr = reinterpret_cast<float*>(const_cast<std::byte*>(tensor.buf.data()));
        size_t offset = 0;
        print_vector(os, ptr, tensor.shape_, 0, offset);
    }
    else
        throw std::runtime_error("Type is not supported");

    return os;
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
{ return shape_; };
const std::byte* Tensor::data() const
{ return buf.data(); }
} // namespace naida