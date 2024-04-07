#pragma once
#include <optional>
#include <atomic>

/// @brief Adapter class over contiguous sequence of objects. Simple analog of std::span from c++20
/// @tparam T Type of the object
template <typename T>
class ArrayView
{
private:
    T* data_;
    std::size_t size_;

public:
    ArrayView(T* data, std::size_t size)
        : data_(data), size_(size)
    {
    }

    T* begin() const
    {
        return data_;
    }

    T* end() const
    {
        return data_ + size_;
    }

    T& operator[](std::size_t index) const
    {
        if (index >= size_)
            throw std::out_of_range("Index out of range");

        return data_[index];
    }

    std::size_t size() const
    {
        return size_;
    }
};
