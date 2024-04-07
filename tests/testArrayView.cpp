#include <gtest/gtest.h>
#include "../ArrayView.h"

// Test ArrayView constructor and size()
TEST(ArrayViewTest, ConstructorAndSize)
{
    int arr[] = {1, 2, 3, 4, 5};
    ArrayView<int> view(arr, 5);
    EXPECT_EQ(view.size(), 5);
}

// Test ArrayView begin() and end()
TEST(ArrayViewTest, BeginAndEnd)
{
    int arr[] = {1, 2, 3, 4, 5};
    ArrayView<int> view(arr, 5);
    EXPECT_EQ(*view.begin(), 1);
    EXPECT_EQ(*(view.end() - 1), 5);
}

// Test ArrayView operator[]
TEST(ArrayViewTest, OperatorIndex)
{
    int arr[] = {1, 2, 3, 4, 5};
    ArrayView<int> view(arr, 5);
    EXPECT_EQ(view[0], 1);
    EXPECT_EQ(view[2], 3);
    EXPECT_EQ(view[4], 5);
    // Test out of range
    EXPECT_THROW(view[5], std::out_of_range);
}
