#include <gtest/gtest.h>
#include "naida/block.hh"
#include "naida/tensor.hh"

TEST(HelloTest, BasicAssertions)
{
    EXPECT_STRNE("hello", "world");
    EXPECT_EQ(7 * 6, 42);
}

TEST(E2E, test_identity)
{
    class MyModel : public naida::Block
    {
    public:
        MyModel()
        {
            auto identity = std::make_unique<naida::Identity>();
            this->register_block("identity", std::move(identity));
        }
        virtual ~MyModel() {}
        virtual std::vector<naida::Tensor> forward(const std::vector<naida::Tensor>& inputs)
        { return blocks["identity"]->forward(inputs); }
    };

    auto dtype = naida::DType::FLOAT32;
    naida::Tensor tensor(naida::Shape { 2, 3 }, dtype);

    MyModel model;
    std::vector<naida::Tensor> outs = model.forward({ tensor });
    EXPECT_EQ(outs.size(), 1);

    naida::Tensor& out = outs[0];

    EXPECT_EQ(tensor.shape(), out.shape());
    const std::byte* in_byte = tensor.data();
    const std::byte* out_byte = outs[0].data();

    for (int i = 0; i < tensor.shape().total_size(); i++)
    {
        EXPECT_EQ(tensor.data()[i], out.data()[i]);
    }
}