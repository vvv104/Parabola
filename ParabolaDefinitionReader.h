#pragma once
#include "Parabola.h"

/// @brief Supplementary class for easy reading creating Parabola definitions from an array of integers represented as strings
/// @tparam Array should have random access iterator
template <typename Array>
class ParabolaDefinitionReader
{
public:
    ParabolaDefinitionReader(const Array& data)
        : data_(data)
    {
    }

    /// @brief Provides next available Parabola definition object from input array or nullopt if there is no more data
    /// @return Optional object of Parabola::Definition.
    std::optional<Parabola::Definition> Provide()
    {
        while (true)
        {
            auto pos = pos_.fetch_add(3, std::memory_order_relaxed);

            if (pos + 3 > data_.size())
                return std::nullopt;

            try
            {
                return Parabola::Definition(std::stoi(data_[pos]), std::stoi(data_[pos + 1]), std::stoi(data_[pos + 2]));
            }
            catch (const std::exception&)
            {
                // ignore format errors
            }
        }
    }

private:
    const Array& data_;
    std::atomic<std::size_t> pos_ = 0;
};
