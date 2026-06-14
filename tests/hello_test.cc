#include <gtest/gtest.h>
#include <vector>
#include "naida/block.hh"
#include "naida/tensor.hh"

// Syntax: TEST(SUITE_NAME, test_mytest)
// Suite means a group of tests that can share common routines/shared objects.

// Ref: https://google.github.io/googletest/primer.html#simple-tests


TEST(HelloTest, BasicAssertions)
{
    EXPECT_STRNE("hello", "world");
    EXPECT_EQ(7 * 6, 42) << "Seven times six is fourty two!";
}

TEST(E2E, TestIdentity)
{
    class MyModel : public naida::Block
    {
    public:
        MyModel(): naida::Block("TestModel")
        {
            auto identity = std::make_unique<naida::Identity>();
            this->register_block("identity", std::move(identity));
        }
        virtual ~MyModel() {}
        virtual std::vector<naida::Tensor> forward(const std::vector<naida::Tensor>& inputs)
        {
            return blocks["identity"]->forward(inputs);
        }
    };

    auto dtype = naida::DType::FLOAT32;
    naida::Tensor tensor(naida::Shape { 2, 3 }, dtype);

    MyModel model;
    std::vector<naida::Tensor> outs = model.forward(std::vector<naida::Tensor> { tensor });
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