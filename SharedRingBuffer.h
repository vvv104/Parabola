#pragma once
#include <array>
#include <atomic>
#include <condition_variable>
#include <mutex>

/// @brief This class implements queue-like container of fixed length with multi-threading syncronization of reading and writing 
///        operations. It provides interface in producer-consumer paradigm supporting many producers and many consumers
/// @tparam T - Type of elements in the container
/// @tparam N - static size of the container
template<class T, std::size_t N>
class SharedRingBuffer
{
private:
    std::array<T, N> data_;
    std::size_t head_ = 0;
    std::size_t tail_ = 0;
    std::size_t count_ = 0;
    std::condition_variable notFull_, notEmpty_;
    std::mutex mtx_;
    std::atomic<bool> endData_{ false };

public:
    /// @brief Puts a value into the container and notifies one of consumers about possibility to take it
    /// @param value 
    void produce(const T& value)
    {
        if (endData_)
            return;
            
        std::unique_lock<std::mutex> lock(mtx_);
        notFull_.wait(lock, [this]() { return N != count_; });
        data_[tail_++] = value;
        tail_ %= N;
        ++count_;
        notEmpty_.notify_one();
    }

    /// @brief Extracts a value from the container and notifies one of producers that there is a space available for insertion
    /// @return If return value is nullopt then the will be no data
    std::optional<T> consume()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        notEmpty_.wait(lock, [this]() { return 0 != count_ || endData_; });

        if (0 == count_ && endData_)
            return std::nullopt;

        T value = data_[head_++];
        head_ %= N;
        --count_;
        notFull_.notify_one();
        return value;
    }

    /// @brief Set a queue to status of 'no more data'
    void setFinished()
    {
        endData_ = true;
        notEmpty_.notify_all();
    }
};